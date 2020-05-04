#include "utils.h"
#include <chrono>
#include <ctime> 
#include <iomanip>

bool components::is_family(std::string component_id, std::string code){
        return component_id.substr(0,2)==code;
    }

std::string components::date_time() {
    auto end = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    char buf[20];
    std::strftime(buf,sizeof(buf),"%Y-%m-%d %H:%M:%S",std::gmtime(&end_time));

    return std::string(buf);

}

