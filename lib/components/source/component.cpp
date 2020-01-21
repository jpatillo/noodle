#include "component.h"
#include <sstream>

component::component(std::string id){
    _id=id;
}
component::~component(){}

std::string component::get_id(){return _id;}

std::string component::get_status(){
    std::ostringstream ostr;
    ostr << "{\"id\":\"" << get_id() << "\"}";
    return ostr.str();
}