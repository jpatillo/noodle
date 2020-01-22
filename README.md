# Noodle Project

The Noodle Project is a pet snake environment controller. The project does things like act as a thermostat for heating elements and record environment humidity and temperature.

The latest (seemingly) stable build is in the dev branch.

*Just a note:* CMake is a real drain on my life.

The device currently publishes data frequently. This is desirable for something like real time reporting. Two minutes seems like an appropriate interval for this project.

## TODO

- Shut off heat sources if the temperature reading fails.
- Shut off heat sources if the program is not running. Supply power to relay from device? Will the relay auto-trip?

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

```bash
#!/usr/bin/env bash

mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=debug -DDAEMON=ON ../
make
sudo make install
/usr/local/bin/noodle
```

### Release

`./build.sh`

## Resources

### Mosquitto

- man <https://mosquitto.org/api/files/mosquitto-h.html#mosquitto_loop_start>
- cpp header reference <https://github.com/iosphere/mosquitto/blob/master/lib/cpp/mosquittopp.h>
- cpp example <https://github.com/eclipse/mosquitto/blob/master/examples/temperature_conversion/main.cpp>
