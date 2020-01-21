/******************************************************************************/
// OneWireDevice
//
// A single instance represents a device connected on the 1-wire bus.
/******************************************************************************/
#ifndef NOODLE_ONEWIREDEVICE_H_
#define NOODLE_ONEWIREDEVICE_H_

#include "component.h"
#include <string>

const std::string ONEWIRE_DRIVER_PATH = "/sys/bus/w1/devices/";

class OneWireDevice : public component {
    std::string driverpath_;

    void BuildDriverPath();

  public:

    OneWireDevice(std::string id);
    virtual ~OneWireDevice();

    // Returns the path to the 1-wire driver
    std::string get_driverpath() const;

};

#endif
