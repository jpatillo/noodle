
project(inih_pp)

add_library(${PROJECT_NAME} STATIC ../source/ini.c source/INIReader.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ../source source)

