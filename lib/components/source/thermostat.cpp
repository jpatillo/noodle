#include "thermostat.h"
#include <sstream>
#include "utils.h"

thermostat::thermostat(DS18B20 temp, relay onoff):component(id_creator(temp,onoff)),_temp(temp),_relay(onoff){}
thermostat::~thermostat(){}
void thermostat::setMaxTemperature(int temp){_hi=temp;}
void thermostat::setMinTemperature(int temp){_lo=temp;}
void thermostat::check(){
    if(_temp.get_rawtemp()>_hi)_relay.off();
    else if(_temp.get_rawtemp()<_lo)_relay.on();
    else ;
}
bool thermostat::isOn(){return _relay.isOn();}
double thermostat::get_temperature(){return _temp.get_celsiustemp();}


std::string thermostat::id_creator(DS18B20 temp, relay onoff){
    std::string t = temp.get_id();
    std::string o = onoff.get_id();
    return "66-"+ t.substr(t.length() - 6) + o.substr(o.length() - 6); 
}

std::string thermostat::get_status(){
    std::ostringstream ostr;
    ostr << "{\"sensor_id\":\"" << get_id() 
        << "\",\"active\":" << _relay.isOn() 
        << ",\"temperature\":" << _temp.get_celsiustemp() 
        << ",\"device_time\":\"" << components::date_time() 
        << "\"}";
    return ostr.str();
}