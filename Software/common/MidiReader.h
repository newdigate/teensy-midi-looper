//
// Created by Nicholas Newdigate on 10/04/2018.
//

#ifndef TEENSY_MIDI_COMMON_MIDIREADER_H
#define TEENSY_MIDI_COMMON_MIDIREADER_H

#if ARDUINO >= 100
#include "Arduino.h"
#include <SD.h>
#else
#include "mock_arduino.h"
#include "SD/SD.h"
#endif

#include <string>

#define MAX_MIDI_TRACKS 16

class MidiReader {
public:
    MidiReader() :
            _filename(),
            _midifile(),
            _initialized(false),
            _track_offset(),
            _track_size()
    {
    }

    bool open(const char* filename);


private:
    bool _initialized;
    File _midifile;
    char* _filename;
    unsigned long _track_size[MAX_MIDI_TRACKS];
    unsigned long _track_offset[MAX_MIDI_TRACKS];
};


#endif //TEENSY_MIDI_COMMON_MIDIREADER_H
