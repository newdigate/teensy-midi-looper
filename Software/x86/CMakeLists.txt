cmake_minimum_required(VERSION 3.9)
project(arduino_midi_writer)

set(CMAKE_CXX_STANDARD 11)

add_executable(arduino_midi_writer
        ../common/cppQueue/cppQueue.cpp
        ../common/cppQueue/cppQueue.h
        ../common/midi/MIDI.cpp
        ../common/midi/MIDI.h
        ../common/midi/MIDI.hpp
        ../common/midi/midi_Defs.h
        ../common/midi/midi_Message.h
        ../common/midi/midi_Namespace.h
        ../common/midi/midi_Settings.h
        mocks/hardware_serial.cpp
        mocks/hardware_serial.h
        mocks/mock_arduino.cpp
        mocks/mock_arduino.h
        runtests.cpp
        ../common/Delegate.h
        ../common/MidiLoopSequencer.cpp
        ../common/MidiLoopSequencer.h
        ../common/MidiWriter.cpp
        ../common/MidiWriter.h)
