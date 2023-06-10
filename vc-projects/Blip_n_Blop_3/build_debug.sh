#!/bin/bash

emcmake cmake -DCMAKE_BUILD_TYPE=Debug
cmake --build . -j 4
