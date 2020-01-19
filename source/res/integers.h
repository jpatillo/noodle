#ifndef NOODLE_RES_INTEGERS_H_
#define NOODLE_RES_INTEGERS_H_

// Time between sensor checks
const unsigned int DEFAULT_SENSOR_INTERVAL = 300; // 5 minutes
const unsigned int MAX_SENSOR_INTERVAL = 86400; // 1 day
const unsigned int MIN_SENSOR_INTERVAL = 15; // 15 seconds

// Time between checking for currently mounted sensors
const unsigned int DEFAULT_MOUNT_INTERVAL = 600; // 10 minutes
const unsigned int MAX_MOUNT_INTERVAL = 86400; // 1 day
const unsigned int MIN_MOUNT_INTERVAL = 300; //5 minutes

// Thermostat defaults.
const unsigned int DEFAULT_MAX_TEMP = 29444; //85F
const unsigned int DEFAULT_MIN_TEMP = 25555; //78F

#endif
