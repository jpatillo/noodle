# - Find libmosquitto
# Find the native libmosquitto includes and libraries
#
#  MOSQUITTO_INCLUDE_DIR - where to find mosquitto.h, etc.
#  MOSQUITTO_LIBRARIES   - List of libraries when using libmosquitto.
#  MOSQUITTO_FOUND       - True if libmosquitto found.

find_library(MOSQUITTO_LIBRARIES NAMES mosquittopp PATHS 
    /usr
    /usr/local
    /opt
    )

find_path(MOSQUITTO_INCLUDE_DIR mosquittopp.h PATHS
      /usr
      /usr/local
      /opt
      PATH_SUFFIXES
      )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  MOSQUITTO DEFAULT_MSG
  MOSQUITTO_LIBRARIES
  MOSQUITTO_INCLUDE_DIR
  )

mark_as_advanced(
  MOSQUITTO_LIBRARIES
  MOSQUITTO_INCLUDE_DIR 
  )