#!/usr/bin/env bash

set -e

BUILD_TYPE="Release"
BUILD_DIR="cmake-build-release"

rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"
cmake -S . -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
cd "$BUILD_DIR"
cmake --build .

mkdir -p ../build
cp ../app/APP ../build
cp -r ../app/resources ../build
cp ../app/config.json ../build
cp ../app/imgui.ini ../build

