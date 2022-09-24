#!/bin/bash

set -e

g++ \
    cplus.cpp \
    -Ivendor/termcolor/include \
    -Ivendor/yaml-cpp/include -Lvendor/yaml-cpp/build -lyaml-cpp \
    --static \
    -o cplus
