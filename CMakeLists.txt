cmake_minimum_required(VERSION 3.9)
project(arduino_midi_writer)

set(CMAKE_CXX_STANDARD 11)

add_executable(arduino_midi_writer
        cppQueue/cppQueue.cpp
        cppQueue/cppQueue.h
        midi/MIDI.cpp
        midi/MIDI.h
        midi/MIDI.hpp
        midi/midi_Defs.h
        midi/midi_Message.h
        midi/midi_Namespace.h
        midi/midi_Settings.h
        tests/hardware_serial.cpp
        tests/hardware_serial.h
        tests/mock_arduino.cpp
        tests/mock_arduino.h
        tests/runtests.cpp
        Delegate.h
        MidiLoopSequencer.cpp
        MidiLoopSequencer.h
        MidiWriter.cpp
        MidiWriter.h)
