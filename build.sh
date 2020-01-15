#!/usr/bin/env bash

# Mosquitto
sudo apt-get install libmosquitto-dev libmosquittopp-dev libssl-dev -y

mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=debug -DDAEMON=OFF ../
make noodle