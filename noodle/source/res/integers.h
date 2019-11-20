#ifndef VIVARIA_RES_INTEGERS_H_
#define VIVARIA_RES_INTEGERS_H_

// Max db rows that can be sent to the web service at one time
const unsigned int WEB_SYNC_ROW_LIMIT = 5;
// Max messages that can be handled per thread loop
const unsigned int WEB_SYNC_MSG_LIMIT = 5;
// Time between sensor checks
const unsigned int DEFAULT_SENSOR_INTERVAL = 300; // 5 minutes
const unsigned int MAX_SENSOR_INTERVAL = 86400; // 1 day
// Time between web service updates
const unsigned int INT_WEB_SYNC_INTERVAL = 0; // no delay
const unsigned int INT_WEB_SYNC_INTERVAL_MAX = 86400; // 1 day

// Time between checking for currently connected sensors
const unsigned int DEFAULT_MOUNT_INTERVAL = 600; // 10 minutes
const unsigned int MAX_MOUNT_INTERVAL = 86400; // 1 day

// These keys will be used as ContentValues keys for identifying data quickly.
const unsigned int KEY_DEVICEID = 0x01;
const unsigned int KEY_MODEL = 0x02;
const unsigned int KEY_MANUFACTURED = 0x03;
const unsigned int KEY_FIRMWARE = 0x04;
const unsigned int KEY_SENSORID = 0x05;
const unsigned int KEY_SENSORTYPE = 0x06;
const unsigned int KEY_DATA = 0x07;
const unsigned int KEY_TEMPERATURE = 0x08;
const unsigned int KEY_CREATED = 0x09;
const unsigned int KEY_SENSORS = 0x0A;

const unsigned int DHT11_FAMILY = 0xD1;
const unsigned int DHT22_FAMILY = 0xD2;

// 1 byte
const unsigned int PROTOCOL_GENERIC = 0x00;
const unsigned int PROTOCOL_1WIRE = 0x01;
const unsigned int PROTOCOL_I2C = 0x04;
const unsigned int PROTOCOL_SPI = 0x08;

#endif
