# Noodle Project

The Noodle Project is a pet snake environment controller. The project does things like act as a thermostat for heating elements and record environment humidity and temperature.

*Just a note:* CMake is a real drain on my life.

## TODO

- Shut off heat sources if the temperature reading fails.
- Shut off heat sources if the program is not running. Supply power to relay from device? Will the relay auto-trip?

## Build

For testing, use run.sh.

### Local testing

set Daemon flag off `-DDAEMON=OFF`

run the install

```bash
make
./noodle
```

### Daemon testing

set Daemon flag on `-DDAEMON=ON`

run the install

```bash
make
sudo make install
```

### Release

`./build.sh`

## Resources

### Mosquitto

- man <https://mosquitto.org/api/files/mosquitto-h.html#mosquitto_loop_start>
- cpp header reference <https://github.com/iosphere/mosquitto/blob/master/lib/cpp/mosquittopp.h>
- cpp example <https://github.com/eclipse/mosquitto/blob/master/examples/temperature_conversion/main.cpp>
