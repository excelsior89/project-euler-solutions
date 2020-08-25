#!/bin/sh

# Copyright 2020 Paul Robertson
#
# Build script for OSX
#
# IMPORTANT: This build script is untested on OSX and
# may therefore contain silly syntax errors (and worse)...
# It is provided as a guide only.

# Ensure the PATH includes locally installed tools
PATH=/usr/local/bin:$PATH

# Create build directory
mkdir -p build_mac
cd build_mac

# Default to release build
build=Release
if [[ $1 == "Debug" ]]; then
    build=Debug
fi

# Run CMake targeting Xcode and build with specified config (Release/Debug)
cmake -G "Xcode" $@ .. || { echo 'CMake generate failed.' ; exit 1; }
cmake --build . --config ${build} --target install || { echo 'Build failed.' ; exit 1; }
cd ..
