#include "utils.h"

bool components::is_family(std::string component_id, std::string code){
        return component_id.substr(0,2)==code;
    }