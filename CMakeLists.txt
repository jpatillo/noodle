cmake_minimum_required(VERSION 2.8.9)
project(noodle)

# Version
set(NOODLE_VERSION_MAJOR 0)
set(NOODLE_VERSION_MINOR 2)



set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")



# check c++11 support
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

OPTION(ENABLE_OPENSSL "Enable use of OpenSSL" OFF)
OPTION(DAEMON "Build Noodle client as daemon" OFF)

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

set(SOURCE_FILES source/main.cpp
	source/ContentValues.cpp
	source/OneWire/DS18B20.cpp
	source/OneWire/OneWireDevice.cpp
	source/OneWire/OneWireManager.cpp
	source/util.cpp )

# set OpenSSL if available
add_definitions( -DOPEN_SSL=${OPEN_SSL} )

# pthreads
find_package(Threads REQUIRED)
if(THREADS_HAVE_PTHREAD_ARG)
	target_compile_options(PUBLIC noodle "-pthread")
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

# Support for WiringPi
find_package(WiringPi)
if(WiringPi_FOUND)
    include_directories(${WiringPi_INCLUDE_DIR})
    list(APPEND ADDITIONAL_LIBS ${WiringPi_LIBRARIES})
else()
	  message(STATUS "WiringPi not found.")
endif()





if(DAEMON)
    # daemon service that can be installed as a linux service
    add_executable(noodle ${SOURCE_FILES})
    target_link_libraries(noodle ${ADDITIONAL_LIBS})
    set_target_properties(noodle PROPERTIES COMPILE_DEFINITIONS "DAEMON=1")

    install(TARGETS noodle RUNTIME DESTINATION bin)
    install(FILES "${CMAKE_SOURCE_DIR}/install/raspberry/noodle" DESTINATION /etc/init.d)
else()
    add_executable(noodle ${SOURCE_FILES})
    target_link_libraries(noodle ${ADDITIONAL_LIBS})
    set_target_properties(noodle PROPERTIES COMPILE_DEFINITIONS "DAEMON=0")
endif()
