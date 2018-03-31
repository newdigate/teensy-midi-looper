cmake_minimum_required(VERSION 3.5)
project(arduino_midi_writer C CXX)

set(CMAKE_CXX_STANDARD 11)

include_directories(../arduino)
include_directories(../common)
link_directories(../arduino/cmake-build-debug)
link_directories(../common/cmake-build-debug)

add_executable(arduino_midi_writer
        runtests.cpp
        AdafruitTFTMock.cpp
        AdafruitTFTMock.h)

target_link_libraries(arduino_midi_writer arduino_abstraction)
target_link_libraries(arduino_midi_writer teensy_midi_common)