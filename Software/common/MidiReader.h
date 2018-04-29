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
#define TRACK_EVENT_BUFFER_SIZE 50
#define TRACK_EVENT_BUFFER_MIN_THRESHOLD 20

struct MidiMessage {
    uint32_t delta_ticks;
    unsigned char status;
    unsigned char key;
    unsigned char velocity;
    unsigned char channel;
};

class MidiReader {
public:
    MidiReader() :
            _filename(),
            _midifile(),
            _initialized(false),
            _track_offset(),
            _track_size(),
            _track_buffer() {
    };

    bool open(const char* filename);
    bool seek(unsigned char trackNumber, unsigned long milliseconds);
    bool read(unsigned char trackNumber);

private:
    bool _initialized;
    File _midifile;
    char* _filename;
    uint16_t _pulses_per_quater_note;
    float _current_beats_per_minute = 120.0;
    unsigned long _track_size[MAX_MIDI_TRACKS];
    unsigned long _track_offset[MAX_MIDI_TRACKS];
    unsigned long _track_position[MAX_MIDI_TRACKS];
    Queue *_track_buffer[MAX_MIDI_TRACKS];

    void readMetaText(uint16_t track_index);

    const char * voice_message_status_name(unsigned char status);
};


#endif //TEENSY_MIDI_COMMON_MIDIREADER_H
