#!/bin/bash

set -e

rm -rf vendor

##### termcolor #####
git clone --depth=1 --branch=master https://github.com/ikalnytskyi/termcolor vendor/termcolor
rm -rf vendor/termcolor/.git 

##### yaml-cpp ######
git clone --depth=1 --branch=master https://github.com/jbeder/yaml-cpp vendor/yaml-cpp
rm -rf vendor/yaml-cpp/.git
mkdir vendor/yaml-cpp/build
cmake -S vendor/yaml-cpp -B vendor/yaml-cpp/build 
cmake --build vendor/yaml-cpp/build -j

g++ \
    --std=c++2a \
    cplus.cpp \
    -Ivendor/termcolor/include \
    -Ivendor/yaml-cpp/include -Lvendor/yaml-cpp/build -lyaml-cpp \
    --static \
    -o bin/cplus.bootstrap

rm -rf vendor
./bin/cplus.bootstrap

rm -f ./bin/cplus.bootstrap
