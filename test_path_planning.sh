#!/bin/bash

g++ -g .\\RunPath.cpp .\\PathPlanning.cpp .\\Grid.cpp -o RunPath.exe

./RunPath

.venv/Scripts/python see_planner.py