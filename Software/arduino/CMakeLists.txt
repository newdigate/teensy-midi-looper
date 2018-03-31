cmake_minimum_required(VERSION 3.5)

SET(CMAKE_CXX_FLAGS_DEBUG "-O0")
SET(CMAKE_C_FLAGS_DEBUG "-O0")

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
        midi/midi_Settings.h
        SD/File.cpp
        SD/SD.cpp
        SD/SD.h
        SD/utility/FatStructs.h
        SD/utility/Sd2Card.cpp
        SD/utility/Sd2Card.h
        SD/utility/Sd2PinMap.h
        SD/utility/SdFat.h
        SD/utility/SdFatUtil.h
        SD/utility/SdFatmainpage.h
        SD/utility/SdFile.cpp
        SD/utility/SdInfo.h
        SD/utility/SdVolume.cpp
        Adafruit/TFTColorHelper.cpp Adafruit/TFTColorHelper.h)
