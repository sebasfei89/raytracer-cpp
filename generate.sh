#!/bin/sh

cmake -S . -B Build -G "Visual Studio 15 2017 Win64" -DCMAKE_TOOLCHAIN_FILE=$vcpkgRoot/scripts/buildsystems/vcpkg.cmake
