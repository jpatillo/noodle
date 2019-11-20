#ifndef NOODLE_RES_STRINGS_H_
#define NOODLE_RES_STRINGS_H_

// Raspberry Pi USERAGENT for HTTP communication
static std::string USERAGENT = "Vivaria RPi 01";
// API method
static std::string TOPIC_TELEMETRY = "postTelemetry";
static std::string TOPIC_SENSOR = "postSensor";
static std::string TOPIC_DEVICE = "postDevice";
// API keys and database columns
static std::string ELEMENT_DEVICEID = "deviceid";
static std::string ELEMENT_MODEL = "model";
static std::string ELEMENT_MANUFACTURED = "manufactured";
static std::string ELEMENT_FIRMWARE = "firmware";
static std::string ELEMENT_SENSORID = "sensorid";
static std::string ELEMENT_SENSORTYPE = "sensortype";
static std::string ELEMENT_DATA = "data";
static std::string ELEMENT_TEMPERATURE = "temp";
static std::string ELEMENT_CREATED = "created";
static std::string ELEMENT_SENSORS = "sensors";

static std::string ELEMENT_URI = "uri";
static std::string ELEMENT_APIPATH = "type";
static std::string ELEMENT_HTTPCODE = "code";
static std::string ELEMENT_MESSAGE = "message";
// Sensors
static std::string SENSOR_DS2401 = "DS2401";
static std::string SENSOR_DS2401_PREFIX = "01";
static std::string SENSOR_DS18B20 = "DS18B20";
static std::string SENSOR_DS18B20_PREFIX = "28";
static std::string SENSOR_DHT11 = "DHT11";
static std::string SENSOR_DHT22 = "DHT22";
// Database
static std::string DB_PATH = "records.db";
static std::string TELEMETRY_TABLE = "telemetry";
static std::string WEB_TABLE = "web";
static std::string SENSOR_TABLE = "sensor";
static std::string CONFIGURATION_TABLE = "configuration";
static std::string RESYNC_TABLE = "resync";
// Add all keys from the config file here for easy access
static std::string GCLOUD_API_ROOT = "GCLOUD_API_ROOT";
static std::string GCLOUD_API_KEY = "GCLOUD_API_KEY";
static std::string THINGSBOARD_URL = "THINGSBOARD_URL";
static std::string THINGSBOARD_TOKEN = "THINGSBOARD_TOKEN";
static std::string VIVARIAHOME_URL = "VIVARIAHOME_URL";
static std::string VIVARIANODE_URL = "VIVARIANODE_URL";
static std::string WEB_SYNC_INTERVAL = "WEB_SYNC_INTERVAL";
static std::string CHECK_SENSORS_INTERVAL = "CHECK_SENSORS_INTERVAL";
static std::string CHECK_MOUNT_INTERVAL = "CHECK_MOUNT_INTERVAL";
static std::string DEVICE_MODEL = "DEVICE_MODEL";
static std::string MANUFACTURE_DATE = "MANUFACTURE_DATE";

#endif
