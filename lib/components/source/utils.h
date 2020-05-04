#ifndef COMPONENT_UTILS_H_
#define COMPONENT_UTILS_H_

#include <string>

namespace components {

    bool is_family(std::string component_id, std::string code);

    /**
     * Returns the system date and time as a string.
     */
    std::string date_time();

}

#endif