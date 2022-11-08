#!/bin/bash

if [ "$1" == "clean" ]; then
  rm -rf ./build/*
fi

if [ "$1" == "run" ]; then
  ./build/bin/*
  else
  mkdir -p build
  cd build
  conan install .. --build=missing
  cmake ..
  cmake --build .
fi