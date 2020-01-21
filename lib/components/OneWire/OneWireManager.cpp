#include "OneWireManager.h"
#include <iostream>
#include <fstream>
#include <dirent.h>     // Directories
#include "DS18B20.h"
#include <wiringPi.h>       // GPIO


using namespace std;

  OneWireManager::OneWireManager(){}
  OneWireManager::~OneWireManager(){
    UnloadDevices();
  }

  void OneWireManager::init(){
     pinMode(ONEWIRE_PIN, INPUT); // WiringPi hardcodes pin 4 to 1-wire
     LoadAvailableDevices();
  }

  void OneWireManager::LoadAvailableDevices() {
    UnloadDevices();
    GetDeviceList();
  }

  void OneWireManager::UnloadDevices() {
    while(!devicelist_.empty()) {
      delete devicelist_.back();
      devicelist_.pop_back();
    }
  }

  // Gets a list of connected devices from the 1-wire bus
  void OneWireManager::GetDeviceList() {
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (ONEWIRE_DRIVER_PATH.c_str())) != NULL) {
      /* print all the files and directories within directory */
      while ((ent = readdir (dir)) != NULL) {
        // Get device type
        string id(ent->d_name);
        if(id=="." || id==".." || id=="w1_bus_master1");
        else if(id.substr(0,2)==SENSOR_DS2401_PREFIX){
          devicelist_.push_back(new DS18B20(id));
        }
        else {
          devicelist_.push_back(new OneWireDevice(id));
        }
      }
    closedir (dir);
    } else {
      /* could not open directory */
      perror ("");
      //  return EXIT_FAILURE;
    }
  } //end getConnectedDevices()

  int OneWireManager::count_devices(){return devicelist_.size();}

  bool OneWireManager::is_family(int index, string code){
    return get_id(index).substr(0,2)==code;
  }

  string OneWireManager::get_family(int index){
    return get_id(index).substr(0,2);
  }

  // This method returns a formatted id that is ready for sync or display.
  // This method does not return the internal id used by the driver.
  string OneWireManager::get_id(int index){
    // TODO use PROTOCOL_1WIRE instead of hard-coded value
    string id = devicelist_.at(index)->get_id();
  //  cout<<"id before: "<<id;
  //  id = "1" + id.erase(2,1);
  //  cout<<" id after: "<<id<<endl;
    return id;
  }

  bool OneWireManager::isDS2401(int index){
    return devicelist_.at(index)->get_id().substr(0,2)==SENSOR_DS2401_PREFIX;
  }
  bool OneWireManager::isDS18B20(int index){
    return devicelist_.at(index)->get_id().substr(0,2)==SENSOR_DS18B20_PREFIX;
  }

  OneWireDevice* OneWireManager::get_device(std::string id){
    return get_device(get_index(id));
  }

  OneWireDevice* OneWireManager::get_device(int index){
    return devicelist_.at(index);
  }

  //TODO validate the id
  int OneWireManager::get_index(string id) {
   // id = id.substr(1,string::npos);
    for(vector<OneWireDevice*>::iterator iterator = devicelist_.begin();iterator!=devicelist_.end();++iterator)
      if((*iterator)->get_id()==id)
        return distance(devicelist_.begin(),iterator);
    return 0;
  }

  void OneWireManager::DebugPrintid(int index){
    cout<<get_id(index)<<endl;
  }


  void OneWireManager::DebugPrintDeviceids(){
    for(vector<OneWireDevice*>::const_iterator iterator = devicelist_.begin();iterator!=devicelist_.end();++iterator)
      cout<<(*iterator)->get_id()<<endl;
  }


