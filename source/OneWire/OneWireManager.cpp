#include "OneWireManager.h"
#include <iostream>
#include <fstream>
#include <dirent.h>     // Directories
#include "DS18B20.h"

using namespace std;

  OneWireManager::OneWireManager(){}
  OneWireManager::~OneWireManager(){
    UnloadDevices();
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
        string serial(ent->d_name);
        if(serial=="." || serial==".." || serial=="w1_bus_master1");
        else if(serial.substr(0,2)==SENSOR_DS2401_PREFIX){
          OneWireDevice *node = new DS18B20(serial);
          devicelist_.push_back(node);
        }
        else {
          OneWireDevice *node = new OneWireDevice(serial);
          devicelist_.push_back(node);
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
    return get_serial(index).substr(0,2)==code;
  }

  string OneWireManager::get_family(int index){
    return get_serial(index).substr(0,2);
  }

  // This method returns a formatted serial that is ready for sync or display.
  // This method does not return the internal serial used by the driver.
  string OneWireManager::get_serial(int index){
    // TODO use PROTOCOL_1WIRE instead of hard-coded value
    string serial = devicelist_.at(index)->get_serial();
  //  cout<<"serial before: "<<serial;
  //  serial = "1" + serial.erase(2,1);
  //  cout<<" serial after: "<<serial<<endl;
    return serial;
  }

  double OneWireManager::get_fahrenheittemp(int index) {
    if(isDS18B20(index))
      return ((DS18B20*)devicelist_.at(index))->get_fahrenheittemp();
    else return -1;
  }

  //TODO return an error condition that can not be a valid temp
  double OneWireManager::get_celsiustemp(int index) {
    if(isDS18B20(index))
      return ((DS18B20*)devicelist_.at(index))->get_celsiustemp();
    else return -1;
  }

  long OneWireManager::get_rawtemp(int index) {
    if(isDS18B20(index))
      return ((DS18B20*)devicelist_.at(index))->get_rawtemp();
    else return -1;
  }

  bool OneWireManager::isDS2401(int index){
    return devicelist_.at(index)->get_serial().substr(0,2)==SENSOR_DS2401_PREFIX;
  }
  bool OneWireManager::isDS18B20(int index){
    return devicelist_.at(index)->get_serial().substr(0,2)==SENSOR_DS18B20_PREFIX;
  }

  OneWireDevice* OneWireManager::get_device(std::string serial){
    return get_device(get_index(serial));
  }

  OneWireDevice* OneWireManager::get_device(int index){
    return devicelist_.at(index);
  }

  //TODO validate the serial
  int OneWireManager::get_index(string serial) {
   // serial = serial.substr(1,string::npos);
    for(vector<OneWireDevice*>::iterator iterator = devicelist_.begin();iterator!=devicelist_.end();++iterator)
      if((*iterator)->get_serial()==serial)
        return distance(devicelist_.begin(),iterator);
    return 0;
  }

  void OneWireManager::DebugPrintSerial(int index){
    cout<<get_serial(index)<<endl;
  }
  void OneWireManager::DebugPrintTemp(int index){
    cout<<get_rawtemp(index)<<endl;
  }


  void OneWireManager::DebugPrintDeviceSerials(){
    for(vector<OneWireDevice*>::const_iterator iterator = devicelist_.begin();iterator!=devicelist_.end();++iterator)
      cout<<(*iterator)->get_serial()<<endl;
  }

  void OneWireManager::DebugPrintDeviceTemps(){
    for(vector<OneWireDevice*>::const_iterator iterator = devicelist_.begin();iterator!=devicelist_.end();++iterator)
        if((*iterator)->get_serial().substr(0,2)==SENSOR_DS2401_PREFIX)
          cout<< dynamic_cast<DS18B20*>(*iterator)->get_rawtemp()<<endl;
  }
