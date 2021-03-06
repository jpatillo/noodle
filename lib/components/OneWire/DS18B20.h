// OneWireDevice.h

#ifndef NOODLE_DS18B20_H_
#define NOODLE_DS18B20_H_

#include "OneWireDevice.h"

class DS18B20: public OneWireDevice {

  public:
    DS18B20(std::string id);
    ~DS18B20();
    double get_fahrenheittemp();
    double get_fahrenheittemp(long t);
    double get_celsiustemp();
    double get_celsiustemp(long t);
    long get_rawtemp();

};

#endif
