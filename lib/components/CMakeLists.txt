
project(components)

set(SOURCE_FILES 
    source/utils.cpp 
    source/component.cpp 
    source/relay.cpp 
    source/thermostat.cpp 
    OneWire/DS18B20.cpp 
    OneWire/OneWireDevice.cpp 
    OneWire/OneWireManager.cpp )

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC source OneWire)