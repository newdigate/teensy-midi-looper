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
		midi/MIDI.h
		midi/MIDI.cpp
		midi/MIDI.hpp
		midi/midi_Defs.h
		midi/midi_Message.h
		midi/midi_Namespace.h
		midi/midi_Settings.h
		Delegate.h)