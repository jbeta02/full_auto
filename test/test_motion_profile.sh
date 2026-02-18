#!/bin/bash

g++ -Iinclude test\\RunProfile.cpp src\\MotionProfile.cpp -o build\\RunProfile.exe

./build/RunProfile

.venv/Scripts/python test\\plot.py