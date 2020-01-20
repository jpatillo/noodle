// Program Entry

#include <iostream>
#include <sstream>
#include <utility>
#include <fstream>

#include <signal.h>         // Handle Ctrl-C
#include <time.h>
#include <unistd.h>
#include <wiringPi.h>       // GPIO
#include <mosquittopp.h>

#include "include/config.h"

#include "res/strings.h"
#include "res/integers.h"
#include "util.h"
#include "mqtt.h"
#include <INIReader.h>
#include <OneWireManager.h> // DS18B20, DS2401
#include <DS18B20.h>
#include "relay.h"
#include "thermostat.h"

using namespace std;// To use or not to use...

#define terminaloutput true

int8_t volatile seagulls = 1; // loop control
time_t sensor_timer, mount_timer;

Mqtt* communicator;
OneWireManager owdevices;
std::vector<relay> relays;
std::vector<thermostat> thermos;

struct conf {
  string device_id;
  string device_model;
  unsigned int sensor_interval = DEFAULT_SENSOR_INTERVAL;
  unsigned int mount_interval = DEFAULT_MOUNT_INTERVAL;
  string broker;
}state;


// Called when user presses Ctrl-C
void interruptHandler(int sig) {
  if(sig==SIGINT) {
    seagulls = 0;
  }
}

// Get the Device Id
int getDId(){
  for(int c=0;c<owdevices.count_devices();c++){
    if(owdevices.is_family(c, SENSOR_DS2401_PREFIX)){
        state.device_id = owdevices.get_serial(c);
        return 0;
    }
  }
  return 1;
}

void appConfig(std::string config_path){
 
  INIReader config_reader(config_path);
 
  if(config_reader.ParseError() || config_reader.GetSections().empty()){
    cout<<"Error reading config.";
    exit(0);
  }

  cout<<"Found "<<config_reader.GetSections().size()<<" sections."<<endl;

  std::set<std::string>::iterator it;
  std::set<std::string> sections = config_reader.GetSections(); //not copying this was giving a segmentation fault
  
  for (it = sections.begin(); it !=sections.end(); ++it)
  {

    if((*it)=="relays"){

      std::set<std::string>::iterator field;
      std::set<std::string> fields = config_reader.GetFields((*it));

      for (field = fields.begin(); field != fields.end(); field++)
      {
        if(int pin = config_reader.GetInteger((*it), (*field),0)){//could just get the value if 'field' and convert to int
         //TODO: more pin validity checks
          relay r(pin);
          relays.push_back(r);
        }
      }
    }
    else if((*it)=="timers"){
      int timz = config_reader.GetInteger((*it),"CHECK_SENSOR_INTERVAL",DEFAULT_SENSOR_INTERVAL);
      cout<<"config sensor interval "<<timz;
      if(timz>MAX_SENSOR_INTERVAL || timz<MIN_SENSOR_INTERVAL)timz=DEFAULT_SENSOR_INTERVAL;
      state.sensor_interval = timz;
      cout<<" saved sensor interval "<< state.sensor_interval;

      timz = config_reader.GetInteger((*it),"CHECK_MOUNT_INTERVAL",DEFAULT_MOUNT_INTERVAL);
      if(timz>MAX_MOUNT_INTERVAL || timz<MIN_MOUNT_INTERVAL)timz=DEFAULT_MOUNT_INTERVAL;
      state.mount_interval = timz;
    }
    else if((*it)=="mqtt"){
      state.broker = config_reader.Get("mqtt","BROKER","");
    }
    else if((*it)=="thermostat"){
      std::set<std::string>::iterator field;
      std::set<std::string> fields = config_reader.GetFields((*it));

      for (field = fields.begin(); field != fields.end(); field++)
      {
        if(int pin = config_reader.GetInteger((*it), (*field),0)){//could just get the value if 'field' and convert to int
          //TODO: more pin validity checks
          thermostat t(*field,pin);
          thermos.push_back(t);
        }
      }
    }
    else if((*it)=="hot"){//will cause an issue if thermostat has not been created
      
    }
    else if((*it)=="cold"){
    
    }
    else {
      cout << "Unknown configuration." << endl;
    }

  }

}


void setup(){
  // Handle ctrl+c
  signal (SIGINT, interruptHandler);

  // Initialize GPIO
  wiringPiSetupGpio();// TODO error check
 
  // Get all sensors on the 1-wire bus
  owdevices.init();

  if(getDId()>0) {
    cerr<<"ERROR: Device Id not found.";
    exit(0);
  }
  
  cout << "Reading config file...";
  appConfig(NOODLE_CONFIG_LOCATION);
  cout <<  "Done." << endl;

  // Get the device model.
  ifstream model(DEVICE_MODEL_PATH);
  getline(model,state.device_model);

  if(state.broker.length()){
    mosqpp::lib_init();//TODO: put this line in the constructor?
    communicator = new Mqtt(state.device_id,state.broker);
  }

  // Start timers
  time(&sensor_timer);
  time(&mount_timer);
}

void loop(){

    // Find out what sensors are currently connected
    if(difftime(time(0),mount_timer) > state.mount_interval) {
      owdevices.LoadAvailableDevices();

      time(&mount_timer);
    }

    // Get sensor values
    if(difftime(time(0),sensor_timer) > state.sensor_interval) {
      // 1-Wire devices
      ostringstream payload; //since we are only reading from DS18B20, we will batch all of their telemetry and publish at once.
      payload <<"[";
      for(int c=0;c<owdevices.count_devices();c++){
        // DS18B20
        if(owdevices.is_family(c, SENSOR_DS18B20_PREFIX)){
          // Timestamp
          char date[20];
          get_formatnowdate(date,20);

          DS18B20* device = (DS18B20*)owdevices.get_device(c);
         
          if(c>0)payload<<",";
          payload << "{\"sensorid\":\"" << owdevices.get_serial(c) << "\",\"temperature\":" << device->get_celsiustemp() << "}";
          
        }
      } 
     

      // Check all the thermostats.
      for(std::vector<thermostat>::iterator i = thermos.begin(); i!=thermos.end(); i++){
        (*i).check();
        //TODO: report status
        payload<<",";
        payload << "{\"sensorid\":\"" << (*i).get_serial() << "\",\"temperature\":" << (*i).get_temperature() << ",\"heating\":" << (*i).isOn() << "}";
      }

       payload<<"]";
      communicator->publish("telemetry",payload.str());

      time(&sensor_timer);
    }

}

void cleanup(){
  if(communicator!=NULL){
    delete communicator;
    mosqpp::lib_cleanup();
  }
}

int main(void) {
  setup();
  while(seagulls) { loop(); }
  cleanup();
  return 0;
}
