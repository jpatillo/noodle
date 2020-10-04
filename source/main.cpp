// Program Entry

#include "main.h"


int main(void) {
  setup();
  while(seagulls) { loop(); }
  cleanup();
  return 0;
}

void interruptHandler(int sig) {
  if(sig==SIGINT) {
    seagulls = 0;
  }
}

int getDId(){
  for(int c=0;c<owdevices.count_devices();c++){
    if(owdevices.is_family(c, SENSOR_DS2401_PREFIX)){
        state.device_id = owdevices.get_id(c);
        return 0;
    }
  }
  return 1;
}

void appConfig(std::string config_path){
  
  INIReader config_reader(config_path);
 
  if(config_reader.ParseError() || config_reader.GetSections().empty()){
    LOG("Error reading config.");
    exit(0);
  }

  std::cout<<"Found "<<config_reader.GetSections().size()<<" sections."<<std::endl;

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
      std::cout<<"config sensor interval "<<timz;
      if(timz>MAX_SENSOR_INTERVAL || timz<MIN_SENSOR_INTERVAL)timz=DEFAULT_SENSOR_INTERVAL;
      state.sensor_interval = timz;
      std::cout<<" saved sensor interval "<< state.sensor_interval;

      timz = config_reader.GetInteger((*it),"CHECK_MOUNT_INTERVAL",DEFAULT_MOUNT_INTERVAL);
      if(timz>MAX_MOUNT_INTERVAL || timz<MIN_MOUNT_INTERVAL)timz=DEFAULT_MOUNT_INTERVAL;
      state.mount_interval = timz;
    }
    else if((*it)=="mqtt"){
      state.broker = config_reader.Get("mqtt","BROKER","");
      state.port = config_reader.GetInteger("mqtt","PORT",1883);
    }
    else if((*it)=="thermostat"){
      std::set<std::string>::iterator field;
      std::set<std::string> fields = config_reader.GetFields((*it));

      for (field = fields.begin(); field != fields.end(); field++)
      {
        //TODO for now we are assuming that all temp sensors are DS18B20
        int index = owdevices.get_index((*field));
        int i = onewire.at(index);
        int pin = config_reader.GetInteger((*it), (*field),0);
        if(!i && pin){
          //TODO more pin validity checks
          thermostat* t = new thermostat(*field,pin);
          widgets.insert( std::pair<std::string,component*>( t->get_id(), t ));
          onewire.at(index) = 1;
        }
      }
     
    }
    else if((*it)=="hot"){//will cause an issue if thermostat has not been created
      
    }
    else if((*it)=="cold"){
    
    }
    else {
      LOG("Unknown configuration.");
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
    std::cerr<<"ERROR: Device Id not found.";
    exit(0);
  }
  
  //initialize flags for adding onewire sensors to widgets
  for(int c=0;c<owdevices.count_devices();c++){
    onewire.push_back(0);
  }

  appConfig(NOODLE_CONFIG_LOCATION);

  // Add OneWire sensors
 for(int c=0;c<owdevices.count_devices();c++){
    if(owdevices.is_family(c, SENSOR_DS18B20_PREFIX) && !onewire.at(c)){
        widgets.insert( std::pair<std::string,component*>(owdevices.get_id(c) , new DS18B20(owdevices.get_id(c)) ));
        onewire.at(c) = 1;
    }
  }
 
  // Get the device model.
  std::ifstream model(DEVICE_MODEL_PATH);
  getline(model,state.device_model);

  if(state.broker.length()) {
    mosqpp::lib_init();//TODO: put this line in the constructor?
    communicator = new Mqtt(state.device_id,state.broker,state.port);
  }

  // Start timers
  time(&sensor_timer);
  time(&mount_timer);
}

void loop(){

  // Get sensor values
  if(difftime(time(0),sensor_timer) > state.sensor_interval) {

    std::ostringstream payload;
    payload <<"[";

    // Check all the components.
    std::map<std::string,component*>::iterator i;
    for(i = widgets.begin(); i!=widgets.end(); i++){
      // Check thermostat temperature and activate heating/cooling
      if(components::is_family(i->first,"66")) {
        ((thermostat*)i->second)->check();
      }
      // Gather the telemetry
      if(i!=widgets.begin())
        payload<<",";
      payload << i->second->get_status();
    }

    payload<<"]";
    LOG(payload.str());
    communicator->publish("telemetry",payload.str());

    time(&sensor_timer);
  }

}

void cleanup(){

  // Widget cleanup
  std::map<std::string,component*>::iterator i;
  for(i = widgets.begin(); i!=widgets.end(); i++){
    delete i->second;
  }
  
  // MQTT cleanup
  if(communicator!=NULL){
    delete communicator;
    mosqpp::lib_cleanup();
  }

}


