#!/bin/bash

g++ -g -Iinclude test\\RunPath.cpp src\\PathPlanning.cpp src\\Grid.cpp -o build\\RunPath.exe

./build/RunPath

.venv/Scripts/python test\\see_planner.py