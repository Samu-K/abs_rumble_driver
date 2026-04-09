# Overview

This project takes in UDP streamed data from Assetto Corsa, and rumbles a motor in the brake pedal to simulate ABS feedback in a real car

## Handling game data

For testing purposes a python script fake_ac.py exists. Running this mocks how Assetto Corsa streams data over UDP.

To get mock telemetry data, you need to start the python script, and send a handshake to connect. See client.cpp in esp-rumble-wsl for specifics.

## esp-rumble-esp
This directory is a esp-idf project, which can be flashed onto an actual board. Untested on hardware currently

Target board is esp32-c3, but using different boards is easy with IBoard interface, see esp32.cpp and esp32.h for examples

## esp-rumble-wsl
This contains a mock board implementation for testing fully simulated

