/**
 * 
 * Initialize WiringPi before using this class.
 * 
 */

#ifndef RELAY_H_
#define RELAY_H_

#include <iomanip>
#include <sstream>
#include <wiringPi.h>       // GPIO
#include "component.h"

class relay : public component {
    unsigned int _pin;
public:
    relay(unsigned int pin);
    ~relay();
    void set_pin(unsigned int pin);
    void on();
    void off();
    bool isOn();
    std::string get_status();
private:
    std::string id_creator(int pin);
};

#endif