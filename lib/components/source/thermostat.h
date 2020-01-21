/**
 * Thermostat 
 * 
 * 
 */
#ifndef THERMOSTAT_H_
#define THERMOSTAT_H_

#include "component.h"
#include "DS18B20.h"
#include "relay.h"

const unsigned int DEFAULT_MAX_TEMP = 29444; //29C 85F
const unsigned int DEFAULT_MIN_TEMP = 25555; //25C 78F

class thermostat : public component {
    DS18B20 _temp;
    relay _relay;
    int _hi=DEFAULT_MAX_TEMP,_lo=DEFAULT_MIN_TEMP;
public:
    thermostat(DS18B20 temp, relay onoff);
    ~thermostat();
    void setMaxTemperature(int temp);
    void setMinTemperature(int temp);
    void check();
    bool isOn();
    double get_temperature();
    std::string get_status();

private:
    std::string id_creator(DS18B20 temp, relay onoff);
};

#endif