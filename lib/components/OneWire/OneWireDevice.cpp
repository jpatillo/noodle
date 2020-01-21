#include "OneWireDevice.h"

  OneWireDevice::OneWireDevice(std::string id):component(id) {
    BuildDriverPath();
  }

  OneWireDevice::~OneWireDevice(){}

  std::string OneWireDevice::get_driverpath() const {
    return driverpath_;
  }

// Private Methods

//TODO make sure id exists and validate
  void OneWireDevice::BuildDriverPath(){
    driverpath_ = ONEWIRE_DRIVER_PATH + _id + "/w1_slave";
  }
