#!/bin/sh

cmake .
if ! make; then
    exit
fi
./pomelotest
