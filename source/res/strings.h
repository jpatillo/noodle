#ifndef NOODLE_RES_STRINGS_H_
#define NOODLE_RES_STRINGS_H_

// API method
static std::string TOPIC_TELEMETRY = "/telemetry";
static std::string TOPIC_SENSOR = "/sensor";
static std::string TOPIC_DEVICE = "/device";
// Add all keys from the config file here for easy access
static std::string CHECK_SENSORS_INTERVAL_KEY = "CHECK_SENSORS_INTERVAL";
static std::string CHECK_MOUNT_INTERVAL_KEY = "CHECK_MOUNT_INTERVAL";


static std::string DEVICE_MODEL_PATH = "/sys/firmware/devicetree/base/model";

static std::string DAEMON_NAME = "noodle";

#endif
