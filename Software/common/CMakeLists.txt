cmake_minimum_required(VERSION 3.5)

project(teensy_midi_common C CXX)

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
        TFTPianoDisplay.h views/MidiLooperMainView.cpp views/MidiLooperMainView.h controls/TFTFlashingIndicator.cpp controls/TFTFlashingIndicator.h)