// Program Entry

#include "main.h"

int main(void) {
 // daemonize();
  setup();
  syslog(LOG_DEBUG,"Enter the loop.");
  while(seagulls) { loop(); sleep(SLEEP_INTERVAL);}
  cleanup();
  return 0;
}

void interruptHandler(int sig) {
  switch(sig)
  {
    case SIGINT:
    syslog(LOG_DEBUG, "Received SIGINT.");
    seagulls = 0;
    break;
    case SIGTERM:
      syslog(LOG_DEBUG, "Received SIGTERM.");
      seagulls = 0;
      break;
  }
}

void initSig(){
    struct sigaction newSigAction;
    sigset_t newSigSet;

    /* Set signal mask - signals we want to block */
    sigemptyset(&newSigSet);
    sigaddset(&newSigSet, SIGCHLD);  /* ignore child - i.e. we don't need to wait for it */
    sigaddset(&newSigSet, SIGTSTP);  /* ignore Tty stop signals */
    sigaddset(&newSigSet, SIGTTOU);  /* ignore Tty background writes */
    sigaddset(&newSigSet, SIGTTIN);  /* ignore Tty background reads */
    sigprocmask(SIG_BLOCK, &newSigSet, NULL);   /* Block the above specified signals */

    /* Set up a signal handler */
    newSigAction.sa_handler = interruptHandler;
    sigemptyset(&newSigAction.sa_mask);
    newSigAction.sa_flags = 0;

    /* Signals to handle */
    sigaction(SIGHUP, &newSigAction, NULL);     /* catch hangup signal */
    sigaction(SIGTERM, &newSigAction, NULL);    /* catch term signal */
    sigaction(SIGINT, &newSigAction, NULL);     /* catch interrupt signal */
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
    syslog(LOG_ERR,"Error reading config.");
    exit(0);
  }

  syslog(LOG_DEBUG,"Found %i config sections.",config_reader.GetSections().size());

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
      int timz = config_reader.GetInteger((*it),CHECK_SENSORS_INTERVAL_KEY,DEFAULT_SENSOR_INTERVAL);
      syslog(LOG_DEBUG,"config sensor interval %d",timz);
      if(timz>MAX_SENSOR_INTERVAL || timz<MIN_SENSOR_INTERVAL)timz=DEFAULT_SENSOR_INTERVAL;
      state.sensor_interval = timz;
      syslog(LOG_DEBUG," saved sensor interval %d", state.sensor_interval);

      timz = config_reader.GetInteger((*it),CHECK_MOUNT_INTERVAL_KEY,DEFAULT_MOUNT_INTERVAL);
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
      syslog(LOG_WARNING,"Unknown configuration.");
    }

  }

}

void setup(){
  // Open syslog
  setlogmask(LOG_UPTO(LOG_INFO));
  openlog(DAEMON_NAME.c_str(), 0, LOG_DAEMON);
  syslog(LOG_DEBUG, "Setting up");

  initSig();
  // Handle ctrl+c
  //signal (SIGINT, interruptHandler);

  // Initialize GPIO
  wiringPiSetupGpio();// TODO error check
 
  // Get all sensors on the 1-wire bus
  owdevices.init();

  if(getDId()>0) {
    syslog(LOG_ERR,"ERROR: Device Id not found.");
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
    syslog(LOG_DEBUG,payload.str().c_str());
    communicator->publish("telemetry",payload.str());

    time(&sensor_timer);
  }

}

void cleanup(){
  syslog(LOG_INFO, "Daemon exiting");
  closelog();

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


