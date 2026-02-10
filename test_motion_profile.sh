#!/bin/bash

g++ .\\produceOut.cpp .\\MotionProfile.cpp -o produceOut.exe

./produceOut

.venv/Scripts/python plot.py