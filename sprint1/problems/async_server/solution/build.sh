#!/bin/bash

if [ "$1" == "clean" ]; then
  rm -rf ./build/*
fi

  mkdir -p build
  cd build
  conan install .. #--build=missing
  cmake ..
  cmake --build .
