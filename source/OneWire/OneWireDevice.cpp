#include "OneWireDevice.h"

  OneWireDevice::OneWireDevice(){}
  OneWireDevice::OneWireDevice(std::string serial) {
    serial_ = serial;
    BuildDriverPath();
  }

  OneWireDevice::~OneWireDevice(){}

  int OneWireDevice::set_serial(std::string serial){
    if(serial_.length()>0) return 1;
    serial_ = serial;
    BuildDriverPath();
    return 0;
  }

  std::string OneWireDevice::get_serial() const {
    return serial_;
  }
  std::string OneWireDevice::get_driverpath() const {
    return driverpath_;
  }

// Private Methods

//TODO make sure serial exists and validate
  void OneWireDevice::BuildDriverPath(){
    driverpath_ = ONEWIRE_DRIVER_PATH + serial_ + "/w1_slave";
  }
