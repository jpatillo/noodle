#include "relay.h"


    relay::relay(unsigned int pin):component(id_creator(pin)){
        set_pin(pin);
    }
   relay::~relay(){off();}
    void relay::set_pin(unsigned int pin){
        _pin = pin;
        pinMode( pin, OUTPUT );
        off();
    }
    void relay::on(){digitalWrite( _pin, LOW );}
    void relay::off(){digitalWrite( _pin, HIGH );}
    bool relay::isOn(){return digitalRead(_pin)==LOW;}

    std::string relay::id_creator(int pin){
        std::ostringstream ostr;
        ostr << "65-relay00000" << std::setw(2) << std::setfill('0') << pin;
        return ostr.str(); 
    }

    std::string relay::get_status(){
        std::ostringstream ostr;
        ostr << "{\"sensor_id\":\"" << get_id() << "\",\"active\":" << isOn() << "}";
        return ostr.str();
    }