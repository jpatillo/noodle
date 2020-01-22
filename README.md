# Noodle Project

The Noodle Project is a pet snake environment controller. The project does things like act as a thermostat for heating elements and record environment humidity and temperature.

The latest (seemingly) stable build is in the dev branch.

*Just a note:* CMake is a real drain on my life.

The device currently publishes data frequently. This is desirable for something like real time reporting. Two minutes seems like an appropriate interval for this project.

## TODO

- Monitor OneWire for changes to connected sensors
  - use inotify on a new thread to check for changes in the mount dir <https://www.thegeekstuff.com/2010/04/inotify-c-program-example/>
- Shut off heat sources if the temperature reading fails.
- Shut off heat sources if the program is not running. Supply power to relay from device? Will the relay auto-trip?
- Status report of software version, device id, connected widgets

## Build

For testing, use run.sh.

### Local testing

```bash
#!/usr/bin/env bash

mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=debug -DDAEMON=OFF ../
make
./noodle
```

### Daemon testing

Local testing should be fine for most instances. Daemon testing just makes sure the installation goes well and the services is configured properly.

```bash
#!/usr/bin/env bash

mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=debug -DDAEMON=ON ../
make
sudo make install
sudo systemctl start noodle
```

### Release

`./build.sh`

## Resources

### Mosquitto

- man <https://mosquitto.org/api/files/mosquitto-h.html#mosquitto_loop_start>
- cpp header reference <https://github.com/iosphere/mosquitto/blob/master/lib/cpp/mosquittopp.h>
- cpp example <https://github.com/eclipse/mosquitto/blob/master/examples/temperature_conversion/main.cpp>
