cmake_minimum_required(VERSION 3.9)
project(arduino_abstraction C CXX)

set(CMAKE_CXX_STANDARD 11)

add_library(arduino_abstraction STATIC
		hardware_serial.cpp
        hardware_serial.h
        mock_arduino.cpp
        mock_arduino.h
        Adafruit/Adafruit_GFX.cpp
        Adafruit/Adafruit_GFX.h
        Adafruit/gfxfont.h
        Teensy/Print.cpp
        Teensy/Print.h
        Adafruit/glcdfont.c
        Teensy/Printable.h
        cppQueue/cppQueue.cpp
        cppQueue/cppQueue.h
        midi/MIDI.h
        midi/MIDI.cpp
        midi/MIDI.hpp
        midi/midi_Defs.h
        midi/midi_Message.h
        midi/midi_Namespace.h
        midi/midi_Settings.h)
