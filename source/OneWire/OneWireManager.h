// OneWireManager
// Maintains a list of devices on the 1-wire bus and provides methods to
// access features of the connected devices.

#ifndef NOODLE_ONEWIREMANAGER_H_
#define NOODLE_ONEWIREMANAGER_H_

#include <string>
#include <vector>
#include "OneWireDevice.h"

class OneWireManager {

  std::vector<OneWireDevice*> devicelist_;

public:

  OneWireManager();
  ~OneWireManager();

  void LoadAvailableDevices();

  void UnloadDevices();

  OneWireDevice* get_device(std::string serial);
  OneWireDevice* get_device(int index);

  // Returns the index of the device with serial in the device list.
  int get_index(std::string serial);

  int count_devices();

  bool is_family(int index, std::string code);

  std::string get_family(int index);

  std::string get_serial(int index);

  double get_fahrenheittemp(int index);

  double get_celsiustemp(int index);

  long get_rawtemp(int index);

  void DebugPrintSerial(int index);
  void DebugPrintTemp(int index);
  void DebugPrintDeviceSerials();
  void DebugPrintDeviceTemps();

protected:

  // Gets a list of connected devices from the 1-wire bus
  void GetDeviceList();

};

#endif
