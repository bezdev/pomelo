#!/bin/sh

cmake -DCMAKE_C_COMPILER=/usr/local/bin/gcc-12 -DCMAKE_CXX_COMPILER=/usr/local/bin/g++-12 CMakeLists.txt
make