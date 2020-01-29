#!/bin/bash

SAMPLE=sample

cd build
cmake ..
make
rm -f Runs/*
./$SAMPLE init_vis.mac
