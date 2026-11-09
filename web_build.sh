#!/bin/bash
set -e
emcmake cmake -B build-web -DCMAKE_BUILD_TYPE=Release
cmake --build build-web -j 4
