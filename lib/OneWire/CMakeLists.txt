
project(OneWire)

add_library(${PROJECT_NAME} STATIC source/DS18B20.cpp source/OneWireDevice.cpp source/OneWireManager.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC source)