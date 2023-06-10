#!/bin/bash

emcmake cmake -DCMAKE_BUILD_TYPE=Release
cmake --build . -j 4
