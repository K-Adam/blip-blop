#!/bin/bash

find ./src \( -name '*.cpp' -o -name '*.h' -o -name '*.hpp' \) -exec clang-format -style=file -i {} \; -exec echo Processed: {} \;
