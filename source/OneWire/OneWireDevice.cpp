#include "OneWireDevice.h"

  OneWireDevice::OneWireDevice(){}
  OneWireDevice::OneWireDevice(std::string serial) {
    serial_ = serial;
    BuildDriverPath();
  }

  OneWireDevice::~OneWireDevice(){}

  void OneWireDevice::set_serial(std::string serial){
    serial_ = serial;
    BuildDriverPath();
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
