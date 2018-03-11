cmake_minimum_required(VERSION 3.9)
project(arduino_abstraction C CXX)

set(CMAKE_CXX_STANDARD 11)

# location of header files
include_directories(../arduino)

add_library(teensy_midi_common STATIC
		MidiLoopSequencer.cpp
		MidiLoopSequencer.h
		MidiWriter.cpp
		MidiWriter.h
		Delegate.h
        TFTPianoDisplay.cpp
        TFTPianoDisplay.h)