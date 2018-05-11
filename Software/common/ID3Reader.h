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
#include <functional>

const char ID3HeaderFlags_None = 0;
const char ID3HeaderFlags_Unsynchronisation = 0x80;
const char ID3HeaderFlags_ExtendedHeader = 0x40;
const char ID3HeaderFlags_ExperimentalIndicator = 0x20;
const char ID3HeaderFlags_FooterPresent = 0x10;

class ID3Header {
public:
    char major_version;
    char revision_number;
    char flags;
    unsigned int tag_size;
};

class ID3Reader {
public:
    ID3Reader() :
            _filename(),
            _file(),
            onID3Tag(),
            _header() {
    };

    bool open(const char* filename);
    std::function<void(char*, char*)> onID3Tag;

private:
    File _file;
    char* _filename;
    ID3Header _header;

    bool readHeader();
    bool readTags();

    unsigned long readUnsigned7bitsX4(const unsigned char *buffer) const;
    unsigned int bytesToUInt64(unsigned char buffer[4]);
};


#endif //TEENSY_MIDI_COMMON_ID3READER_H
