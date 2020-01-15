#!/usr/bin/env bash
mkdir -p build


cp install/config build/config



cd build
cmake -DCMAKE_BUILD_TYPE=debug -DDAEMON=OFF ../
make noodle
./noodle
