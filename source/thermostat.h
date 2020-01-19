/**
 * Thermostat 
 * 
 * 
 */

#include "DS18B20.h"
#include "relay.h"
#include "res/integers.h"

class thermostat {
    DS18B20 _temp;
    relay _relay;
    int _hi=DEFAULT_MAX_TEMP,_lo=DEFAULT_MIN_TEMP;
public:
    thermostat(DS18B20 temp, relay onoff):_temp(temp),_relay(onoff){}
    ~thermostat(){}
    void setMaxTemperature(int temp){_hi=temp;}
    void setMinTemperature(int temp){_lo=temp;}
    void check(){
        if(_temp.get_rawtemp()>_hi)_relay.off();
        else if(_temp.get_rawtemp()<_lo)_relay.on();
        else ;
    }
    bool isOn(){return _relay.isOn();}
};