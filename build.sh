#!/usr/bin/env bash

# Get system updates
#sudo apt-get update
#sudo apt-get upgrade

# Install Mosquitto
#sudo apt-get install libmosquitto-dev libmosquittopp-dev libssl-dev -y

# Shutdown and cleanup old installation
systemctl stop noodle
rm -r build

mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DDAEMON=ON ../
make noodle
make install

systemctl start noodle
systemctl enable noodle