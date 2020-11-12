#include <sstream>
#include <utility>
#include <fstream>
#include <string>

#include <sys/stat.h>
#include <syslog.h>
#include <signal.h>         // Handle Ctrl-C
#include <time.h>
#include <unistd.h>
#include <wiringPi.h>       // GPIO
#include <mosquittopp.h>

#include "include/config.h"

#include "res/strings.h"
#include "res/integers.h"
#include "util.h"
#include "mqtt.h"
#include <INIReader.h>
#include <OneWireManager.h> // DS18B20, DS2401
#include <DS18B20.h>
#include "relay.h"
#include "thermostat.h"
#include <map>
#include "utils.h"

#define DEBUG 0


int8_t volatile seagulls = 1; // loop control
time_t sensor_timer, mount_timer;

Mqtt* communicator;
OneWireManager owdevices;
std::vector<relay> relays;

std::map<std::string,component*> widgets;
// Flags to make sure onewire devices are not included in multiple widgets
std::vector<int> onewire;

struct conf {
  std::string device_id;
  std::string device_model;
  unsigned int sensor_interval = DEFAULT_SENSOR_INTERVAL;
  unsigned int mount_interval = DEFAULT_MOUNT_INTERVAL;
  std::string broker;
  unsigned int port;
}state;


/** 
 * Handle keyboard interrupts.
 * Ctrl+c - quit
 */
void interruptHandler(int sig);
void daemonize();
/** Get Device ID */
int getDId();
/** Read config file and set application state. */
void appConfig(std::string config_path);
/** Run application startup routines. */
void setup();
/** Main application loop. */
void loop();
/** Free resources, gracefully shutdown. */
void cleanup();