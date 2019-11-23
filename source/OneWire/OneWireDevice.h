/******************************************************************************/
// OneWireDevice
//
// A single instance represents a device connected on the 1-wire bus.
/******************************************************************************/
#ifndef NOODLE_ONEWIREDEVICE_H_
#define NOODLE_ONEWIREDEVICE_H_

#include <string>

const std::string ONEWIRE_DRIVER_PATH = "/sys/bus/w1/devices/";

class OneWireDevice {
    std::string serial_;
    std::string driverpath_;

    void BuildDriverPath();

  public:
    OneWireDevice();
    OneWireDevice(std::string serial);
    virtual ~OneWireDevice();

    // Sets the device's serial number. Use this method if it has not already
    // been set in the constructor OneWireDevice(std::string serial)
    void set_serial(std::string serial);

    // Return the device's serial number
    std::string get_serial() const;

    // Returns the path to the 1-wire driver
    std::string get_driverpath() const;

};

#endif
