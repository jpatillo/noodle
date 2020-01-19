/**
 * 
 * Initialize WiringPi before using this class.
 * 
 */

#ifndef RELAY_H_
#define RELAY_H_

#include <wiringPi.h>       // GPIO

class relay {
    unsigned int _pin;
public:
    relay(){}
    relay(unsigned int pin){
        set_pin(pin);
    }
    ~relay(){off();}
    void set_pin(unsigned int pin){
        _pin = pin;
        pinMode( pin, OUTPUT );
        off();
    }
    void on(){digitalWrite( _pin, LOW );}
    void off(){digitalWrite( _pin, HIGH );}
    bool isOn(){return digitalRead(_pin)==LOW;}
};

#endif