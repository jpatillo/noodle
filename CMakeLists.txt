cmake_minimum_required(VERSION 3.13.4)
project(noodle)

# check C++11 support
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()


#######################################################################################
# Command Line Args
#######################################################################################

option(ENABLE_OPENSSL "Enable use of OpenSSL" OFF)
option(DAEMON "Build Noodle client as daemon" OFF)

#######################################################################################
# Config
#######################################################################################

# Version
set(NOODLE_VERSION_MAJOR 0)
set(NOODLE_VERSION_MINOR 2)

if(DAEMON)
  set(NOODLE_CONFIG_LOCATION /etc/${PROJECT_NAME})
else()
  set(NOODLE_CONFIG_LOCATION ".")
endif()

# Create custom header file with build info.
configure_file(source/config.h.in include/config.h)

#######################################################################################
# Library Checks
#######################################################################################
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

# Require WiringPi
find_package(WiringPi REQUIRED)
include_directories(${WiringPi_INCLUDE_DIR})
list(APPEND ADDITIONAL_LIBS ${WiringPi_LIBRARIES})
message(STATUS "WiringPi: ${WiringPi_INCLUDE_DIR} ${WiringPi_LIBRARIES}")

# Find OpenSSL
IF(ENABLE_OPENSSL)
	find_package(OpenSSL REQUIRED)
  if(OPENSSL_FOUND)
  	SET(OPEN_SSL 1)
    include_directories(${OPENSSL_INCLUDE_DIR})
    list(APPEND ADDITIONAL_LIBS ${OPENSSL_LIBRARIES})
    message(STATUS "OpenSSL Version: ${OPENSSL_VERSION}")
  else()
    SET(OPEN_SSL 0)
  endif()
ELSE()
  SET(OPEN_SSL 0)
ENDIF()
# set OpenSSL if available
add_definitions( -DOPEN_SSL=${OPEN_SSL} )

# Mosquitto MQTT client
find_package(Mosquitto REQUIRED)
include_directories(${MOSQUITTO_INCLUDE_DIR})
list(APPEND ADDITIONAL_LIBS ${MOSQUITTO_LIBRARIES})
message(STATUS "libmosquitto: ${MOSQUITTO_INCLUDE_DIR} ${MOSQUITTO_LIBRARIES}")

# pthreads
find_package(Threads REQUIRED)
if(THREADS_HAVE_PTHREAD_ARG)
	target_compile_options(PUBLIC ${PROJECT_NAME} "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
	list(APPEND ADDITIONAL_LIBS ${CMAKE_THREAD_LIBS_INIT})
endif()

# cURL
find_package(CURL)
if(CURL_FOUND)
	include_directories(${CURL_INCLUDE_DIR})
	list(APPEND ADDITIONAL_LIBS ${CURL_LIBRARIES})
endif()


#######################################################################################
# Locate files for build.
#######################################################################################


set(SOURCE_FILES source/main.cpp
	source/util.cpp
  source/mqtt.cpp )

add_subdirectory(lib/inih/cpp)
add_subdirectory(lib/components)


#######################################################################################
# Build/Install
#######################################################################################

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${ADDITIONAL_LIBS})
target_link_libraries(${PROJECT_NAME} inih_pp)
target_link_libraries(${PROJECT_NAME} components)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR})

if(DAEMON)
  set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS "DAEMON=1")

  install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
  install(FILES "${CMAKE_SOURCE_DIR}/install/config" DESTINATION ${NOODLE_CONFIG_LOCATION})
  install(FILES "${CMAKE_SOURCE_DIR}/install/noodle.service" DESTINATION /etc/systemd/system)

else()
  # what is this line doing?
  set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS "DAEMON=0")

  # Copy the config file
  add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
      ${CMAKE_SOURCE_DIR}/install/config
      ${CMAKE_CURRENT_BINARY_DIR}/${NOODLE_CONFIG_LOCATION}
  )
endif()

