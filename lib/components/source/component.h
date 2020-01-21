
#ifndef COMPONENT_H_
#define COMPONENT_H_

#include <string>

class component {

protected:
    std::string _id;
public:
    component(std::string id);
    ~component();

    std::string get_id();
    virtual std::string get_status();

};

#endif