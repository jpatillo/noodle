// Program Entry

#include <iostream>
#include <utility>

#include <signal.h>         // Handle Ctrl-C
#include <time.h>
#include <unistd.h>
#include <wiringPi.h>       // GPIO
#include <mosquittopp.h>

#include "ContentValues.h"  // Message data - map<string,string>
#include "SimpleConfig.h"   // Read config file
#include "OneWire/OneWireManager.h" // DS18B20, DS2401
#include "OneWire/DS18B20.h"
#include "res/strings.h"
#include "res/integers.h"
#include "util.h"
#include "mqtt.h"

using namespace std;

#define RELAY_PIN_1 18
#define ONEWIRE_PIN 4
#define RELAY_PIN_2 23

#define NOODLE_config_file_path "config"

OneWireManager owdevices;
string deviceId;
int8_t volatile seagulls = 1; // loop control
time_t sensor_timer, web_timer, mount_timer;
unsigned int sensor_interval = DEFAULT_SENSOR_INTERVAL;
unsigned int mount_interval = DEFAULT_MOUNT_INTERVAL;
bool terminaloutput = true;
string dmodel, manufactured;
Mqtt* communicator;

// Called when user presses Ctrl-C
void interruptHandler(int sig) {
    if(sig==SIGINT) {
      if(terminaloutput)
        cout<<endl<<"Received kill signal."<<endl;

      seagulls = 0;
  }
}

// Get the Device Id
int getDId(){
  for(int c=0;c<owdevices.count_devices();c++){
    if(owdevices.is_family(c, SENSOR_DS2401_PREFIX)){
        deviceId = owdevices.get_serial(c);
        return 0;
    }
  }
  return 1;
}

void appConfig(SimpleConfig privatedata){
  if(privatedata.hasKey(CHECK_SENSORS_INTERVAL)){
    //TODO catch error if the value can not be converted to int
    unsigned int e = privatedata.getintvalue(CHECK_SENSORS_INTERVAL);
    if(e>MAX_SENSOR_INTERVAL)
      sensor_interval = DEFAULT_SENSOR_INTERVAL;
    else
      sensor_interval = e;
  }

  if(privatedata.hasKey(CHECK_MOUNT_INTERVAL)){
    //TODO catch error if the value can not be converted to int
    unsigned int e = privatedata.getintvalue(CHECK_MOUNT_INTERVAL);
    if(e>MAX_MOUNT_INTERVAL)
      mount_interval = DEFAULT_MOUNT_INTERVAL;
    else
      mount_interval = e;
  }

}

void setRelayPin(unsigned int pin){
  pinMode( pin, OUTPUT );
}

void activateRelay(unsigned int pin) {
  digitalWrite( pin, LOW );
}

void deactivateRelay(unsigned int pin) {
  digitalWrite( pin, HIGH );
}

void setup(){
  // Handle ctrl+c
  signal (SIGINT, interruptHandler);

  // Initialize GPIO
  wiringPiSetupGpio();// TODO error check
  pinMode(ONEWIRE_PIN, INPUT); // WiringPi hardcodes pin 4 to 1-wire
  setRelayPin(RELAY_PIN_1);
  setRelayPin(RELAY_PIN_2);


  // Get all sensors on the 1-wire bus
  owdevices.LoadAvailableDevices();
  if(getDId()>0) {
    cerr<<"ERROR: Device Id not found.";
    exit(0);
  }
  
  SimpleConfig privatedata;
  privatedata.setpath(NOODLE_config_file_path);
  appConfig(privatedata);
  

  // Get the device model.
  ifstream model("/sys/firmware/devicetree/base/model");
  getline(model,dmodel);


  mosqpp::lib_init();//TODO: put this line in the constructor?
  communicator = new Mqtt(deviceId.c_str(),"jamespatillo.com");

  // Start timers
  time(&sensor_timer);
  time(&mount_timer);
}

void loop(){

    // Find out what sensors are currently connected
    if(difftime(time(0),mount_timer) > mount_interval) {
      owdevices.LoadAvailableDevices();

      time(&mount_timer);
    }

    // Get sensor values
    if(difftime(time(0),sensor_timer) > sensor_interval) {
      // 1-Wire devices
      for(int c=0;c<owdevices.count_devices();c++){
        // DS18B20
        if(owdevices.is_family(c, SENSOR_DS18B20_PREFIX)){
          // Timestamp
          char date[20];
          get_formatnowdate(date,20);

          DS18B20* device = dynamic_cast<DS18B20*>(owdevices.get_device(c));

          //warm side
          if(device->get_serial()=="28-000006de3271") {
            cout<<"Warm ";
            if(device->get_rawtemp()>29444) //85F
              deactivateRelay(RELAY_PIN_1);
            if(device->get_rawtemp()<25555) //78F
              activateRelay(RELAY_PIN_1);
          }

          //cool side
          if(device->get_serial()=="28-000006ddc23c") {
            cout<<"Cool ";
            if(device->get_rawtemp()>21111) //70F
              deactivateRelay(RELAY_PIN_2);
            if(device->get_rawtemp()<15555) //60F
              activateRelay(RELAY_PIN_2);
          }


          string topic = "noodle/" + deviceId + "/telemetry";
          string payload = "{sensorid:" + device->get_serial() + ",temperature:" + to_string(device->get_celsiustemp()) + "}";

          communicator->publish(topic,payload);

          // Print to screen
          //if(terminaloutput)
          cout<<date<<"\t|\t"<<device->get_serial()<<" \t|\t "<<device->get_celsiustemp()<<"C"<<endl;
          
        }
      } // End 1-Wire devices

      time(&sensor_timer);
    } // End get sensor values

}

int main(void) {
  setup();
  while(seagulls) { loop(); }

  deactivateRelay(RELAY_PIN_1);
  deactivateRelay(RELAY_PIN_2);
  delete communicator;
  mosqpp::lib_cleanup();

  return 0;
} // main
