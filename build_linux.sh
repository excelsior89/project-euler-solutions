#!/usr/bin/env bash

# Copyright 2020-2022 Paul Robertson
#
# Build script for Linux
#
# IMPORTANT: This build script is untested on any Linux systems
# and may therefore contain silly syntax errors (and worse)...
# It is provided as a guide only.

# Report failure of script for any command failure
set -e

# Create build directory
mkdir -p build_linux
cd build_linux

# Default to release build
build=Release
if [[ $1 == "Debug" ]]; then
    build=Debug
fi

# Run CMake targeting makefiles and build with specified config (Release/Debug)
cmake -D CMAKE_BUILD_TYPE=${build} -G "Unix Makefiles" $@ ..
cmake --build . --target install -j$(nproc)
cd ..
