// OneWireManager
// Maintains a list of devices on the 1-wire bus and provides methods to
// access features of the connected devices.

#ifndef NOODLE_ONEWIREMANAGER_H_
#define NOODLE_ONEWIREMANAGER_H_

#include <string>
#include <vector>
#include "OneWireDevice.h"

#define ONEWIRE_PIN 4 // hard coded from wiringPi

const std::string SENSOR_DS2401 = "DS2401";
const std::string SENSOR_DS2401_PREFIX = "01";
const std::string SENSOR_DS18B20 = "DS18B20";
const std::string SENSOR_DS18B20_PREFIX = "28";

class OneWireManager {

  std::vector<OneWireDevice*> devicelist_;

public:

  OneWireManager();
  ~OneWireManager();

  /* Call init before using any other method. Make sure WiringPi has been started. */
  void init();

  void LoadAvailableDevices();

  void UnloadDevices();

  OneWireDevice* get_device(std::string id);
  OneWireDevice* get_device(int index);

  // Returns the index of the device with id in the device list.
  int get_index(std::string id);

  int count_devices();

  bool is_family(int index, std::string code);

  std::string get_family(int index);

  std::string get_id(int index);

  void DebugPrintid(int index);
  void DebugPrintTemp(int index);
  void DebugPrintDeviceids();
  void DebugPrintDeviceTemps();

protected:

  // Gets a list of connected devices from the 1-wire bus
  void GetDeviceList();
  bool isDS2401(int index);
  bool isDS18B20(int index);

};

#endif