//
// Created by Nicholas Newdigate on 10/04/2018.
//

#ifndef TEENSY_MIDI_COMMON_ID3READER_H
#define TEENSY_MIDI_COMMON_ID3READER_H

#if ARDUINO >= 100
#include "Arduino.h"
#include <SD.h>
#else
#include "mock_arduino.h"
#include "SD/SD.h"
#endif

#include <string>

class ID3Reader {
public:
    ID3Reader() :
            _filename(),
            _file(),
            onID3Tag(){
    };

    bool open(const char* filename);
    std::function<void(char*, char*)> onID3Tag;

private:
    File _file;
    char* _filename;
};


#endif //TEENSY_MIDI_COMMON_ID3READER_H
