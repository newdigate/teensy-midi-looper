//
// Created by Nicholas Newdigate on 10/04/2018.
//

#include "MidiReader.h"

#if ARDUINO >= 100
#include "Arduino.h"
#include <SD.h>
#include <midi_Message.h>
#else
#include "../arduino/mock_arduino.h"
#include "SD/SD.h"
#include "midi/midi_Message.h"
#endif

#define buffer_size 50
//#pragma pack(push, r1, 1)
struct smf_header {
    uint16_t format;
    // 0 = single track file format
    // 1 = multiple track file format
    // 2 = multiple song file format (i.e., a series of type 0 files
    uint16_t num_tracks;
    uint16_t division;
};
//#pragma pack(pop, r1)
uint32_t varfieldGet(File &file, uint16_t &progress);

bool MidiReader::open(const char *filename) {
    if (!SD.exists(filename))
        return false;

    _midifile = SD.open(filename);

    if (!_midifile)
        return false;

    unsigned char buffer[6];
    if (_midifile.read(buffer, 4) != 4) {
        return false;
    }

    if (buffer[0] != 'M' || buffer[1] != 'T' || buffer[2] != 'h' || buffer[3] != 'd')
        return false;

    if (_midifile.read(buffer, 4) != 4) {
        return false;
    }

    auto header_length = static_cast<unsigned long>(buffer[0] << 24 | buffer[1] << 16 | buffer[2] << 8 | buffer[3]);
    if (header_length != 6) {
        return false;
    }

    smf_header header = smf_header();
    // big-endian SMF doesn't allow reading directly to a struct
    //_midifile.read((char*)&buffer, header);

    _midifile.read(buffer, 2);
    header.format = static_cast<uint16_t>(buffer[0] << 8 | buffer[1]);

    _midifile.read(buffer, 2);
    header.num_tracks = static_cast<uint16_t>(buffer[0] << 8 | buffer[1]);

    _midifile.read(buffer, 2);
    header.division = static_cast<uint16_t>(buffer[0] << 8 | buffer[1]);

    if (header.format != 1)
        return false;

    for (uint16_t i=0; i < header.num_tracks; i++) {
        _midifile.read(buffer, 4);

        if (buffer[0] != 'M' || buffer[1] != 'T' || buffer[2] != 'r' || buffer[3] != 'k')
            return false;

        _midifile.read(buffer, 4);
        auto track_length = static_cast<unsigned long>(buffer[0] << 24 | buffer[1] << 16 | buffer[2] << 8 | buffer[3]);;

        _track_offset[i] = _midifile.position();
        _track_size[i] = track_length;
        //_midifile.seek(track_length);

        _track_buffer[i] = new Queue(sizeof(MidiMessage), TRACK_EVENT_BUFFER_SIZE, FIFO);
        _track_position[i] = 0;

        while (_track_position[i] < track_length && !_track_buffer[i]->isFull()) {

            uint16_t progress = 0;
            uint32_t delta_ticks = varfieldGet(_midifile, progress);
            _track_position[i] += progress;

            _midifile.read(buffer, 1);
            _track_position[i]++;
            unsigned char status_byte = buffer[0];
            unsigned char key, velocity, nextByte;
            uint32_t length = 0 ;
            MidiMessage midiMessage;
            char *text;
            switch (status_byte & 0xF0) {
                // Channel Voice Messages ( 3 bytes )
                case 0x80:
                case 0x90:
                case 0xA0:  // Polyphonic Key Pressure (Aftertouch).
                case 0xB0:  // Control Change (and/or Channel Mode Message)
                case 0xC0:  // Program Change
                case 0xD0:  // Channel Pressure (After-touch).
                case 0xE0:  // Pitch Wheel Change.
                {
                    //bool noteOn = (status_byte & 0xF0 == 0x80);
                    //unsigned char channel = status_byte & 0x0F;

                    _midifile.read(buffer, 1);
                    _track_position[i]++;
                    key = buffer[0];

                    _midifile.read(buffer, 1);
                    _track_position[i]++;
                    velocity = buffer[0];

                    midiMessage = MidiMessage();
                    midiMessage.status = status_byte;
                    midiMessage.delta_ticks = delta_ticks;
                    midiMessage.key = key;
                    midiMessage.velocity = velocity;

                    _track_buffer[i]->push(&midiMessage);
                    break;
                }

                case 0xF0: { // System Common Messages, and System Real-Time Messages
                    switch (status_byte) {
                        case 0xF0 : {
                            progress = 0;
                            length = varfieldGet(_midifile, progress);
                            _track_position[i] += progress;
                            _midifile.seek(length);
                            _track_position[i] += length;
                            break;
                        }

                        case 0xFF : {
                            nextByte = _midifile.read();

                            switch (nextByte & 0x0F) {
                                case 00: {// Sequence Number
                                    break;
                                }

                                default: {
                                    readMetaText(i);
                                    break;
                                }

                            }
                            break;
                        }

                        default: {
                            break;
                        }

                    }
                    break;

                }
                default: {
                    break;
                }

            }
            break;
        }

    }
    _pulses_per_quater_note = header.division;


    return true;
}

void MidiReader::readMetaText(uint16_t track_index) {
    uint16_t progress = 0;
    uint32_t length = varfieldGet(_midifile, progress);
    _track_position[track_index] += progress;
    char *text = new char[length];
    _midifile.read(text, length);
    delete[] text;
//                                    _midifile.seek(length);
    _track_position[track_index] += length;
}

bool MidiReader::read(unsigned char trackNumber) {
    return false;
}

uint32_t varfieldGet(File &file, uint16_t &progress)
{
    uint32_t ret = 0;
    uint8_t byte_in;

    for (;;)
    {
        if (file.available()) {
            // return error
            byte_in = file.read();
            progress++;
        }
        ret = (ret << 7) | (byte_in & 0x7f);
        if (!(byte_in & 0x80))
            return ret;
    }
}


bool MidiReader::seek(unsigned char trackNumber, unsigned long milliseconds) {

    float quarter_note = _current_beats_per_minute * milliseconds / (1000 * 60);
    unsigned long ticks = quarter_note * _pulses_per_quater_note;
    Serial.print(quarter_note);

    return false;
}
