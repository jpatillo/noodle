#!/usr/bin/env bash

mkdir -p build
cd build

cmake -DCMAKE_BUILD_TYPE=debug -DDAEMON=ON ../

make
sudo make install

#./noodle

