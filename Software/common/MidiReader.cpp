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
        uint8_t channel = 0;
        unsigned char status_byte = 0;
        bool running_status = false;
        while (_track_position[i] < track_length && !_track_buffer[i]->isFull()) {

            //Serial.printf("position: %d\n", _track_position[i] + _track_offset[i]);

            uint16_t progress = 0;
            uint32_t delta_ticks = varfieldGet(_midifile, progress);
            _track_position[i] += progress;

            _midifile.read(buffer, 1);
            unsigned char new_status_byte = buffer[0];
            _track_position[i]++;

            if (new_status_byte >= 0x80) {
                status_byte = new_status_byte;
                channel = new_status_byte & 0x0f;
                running_status = false;
            } else
                running_status = true;

            unsigned char key, velocity, nextByte;
            uint32_t length = 0;
            if (status_byte < 0xF0) {
                bool read_second_byte = true;
                switch (status_byte >> 4) {
                    // Channel Voice Messages ( 3 bytes / or 2 bytes )
                    //case 0x80:
                    //case 0x90:
                    //case 0xA0:  // Polyphonic Key Pressure (Aftertouch).
                    //case 0xD0:  // Channel Pressure (After-touch).
                    //case 0xE0:  // Pitch Wheel Change.
                    case 0xC:   // Control Change (and/or Channel Mode Message
                    case 0xD: {  // Program Change
                        read_second_byte = false;
                        break;
                    }

                    default:
                        break;

                }
                //bool noteOn = (status_byte & 0xF0 == 0x80);
                //unsigned char channel = status_byte & 0x0F;

                if (running_status)
                    key = new_status_byte;
                else {
                    _midifile.read(buffer, 1);
                    _track_position[i]++;
                    key = buffer[0];
                }

                if (read_second_byte) {
                    _midifile.read(buffer, 1);
                    _track_position[i]++;
                    velocity = buffer[0];
                } else
                    velocity = 0;

                MidiMessage midiMessage = MidiMessage();
                midiMessage.status = status_byte;
                midiMessage.delta_ticks = delta_ticks;
                midiMessage.key = key;
                midiMessage.velocity = velocity;
                midiMessage.channel = channel;

                _track_buffer[i]->push(&midiMessage);
                Serial.printf("delta: 0x%04x (%04d) \t\t\tst:(%02x): %s \t\t\t ch:%02d \tkey:%03d \tvel:%03d\n", midiMessage.delta_ticks, midiMessage.delta_ticks,
                              midiMessage.status, voice_message_status_name(midiMessage.status), midiMessage.channel+1, midiMessage.key, midiMessage.velocity);
            } else {

                switch (status_byte) {
                    case 0xF0 : {
                        progress = 0;
                        length = varfieldGet(_midifile, progress);
                        Serial.printf("sysex length:%d\n",length);
                        _track_position[i] += progress;
                        _midifile.seek(length);
                        _track_position[i] += length;
                        break;
                    }

                    case 0xFF : {
                        nextByte = _midifile.read();
                        _track_position[i]++;

                        switch (nextByte) {
                            case 0x00: {// Sequence Number
                                Serial.printf("sequence number:\n");
                                break;
                            }
                            case 0x2F: {
                                // End of Track -> FF 2F 00
                                uint8_t nn =_midifile.read();
                                Serial.printf("end of track\n");
                                _track_position[i]++;
                                break;
                            }
                            case 0x51: {
                                // FF 51 03 tttttt Set Tempo
                                char c = _midifile.read();
                                _track_position[i]++;
                                if (c == 3) {
                                    // nn dd cc bb
                                    uint8_t nn =_midifile.read();
                                    uint8_t dd =_midifile.read(); // denomiator = 2^dd
                                    uint8_t cc =_midifile.read(); //
                                    uint64_t microseconds_per_quarter_note = nn << 16 | dd << 8 | cc;
                                    float millieconds_per_quarter_note = microseconds_per_quarter_note / 1000;
                                    float bpm = 60000 / millieconds_per_quarter_note;
                                    Serial.printf("tempo : %f\n", bpm);
                                    _track_position[i]+=3;
                                }
                                break;
                            }
                            case 0x54: {
                                // FF 54 05 hr mn se fr ff SMPTE Offset
                                char c = _midifile.read();
                                _track_position[i]++;
                                if (c == 5) {
                                    uint8_t hr =_midifile.read();
                                    uint8_t mn =_midifile.read();
                                    uint8_t se =_midifile.read();
                                    uint8_t fr =_midifile.read();
                                    uint8_t ff =_midifile.read();
                                    Serial.printf("SMPTE Offset : %02d:%02d:%02d:%02d:%02d\n", hr, mn, se, fr, ff);
                                    _track_position[i]+=5;
                                }

                                break;
                            }
                            case 0x58:  {
                                // 04 - Time Signature
                                char c = _midifile.read();
                                _track_position[i]++;
                                if (c == 4) {
                                    // nn dd cc bb
                                    uint8_t nn =_midifile.read();
                                    uint8_t dd =_midifile.read(); // denomiator = 2^dd
                                    uint8_t cc =_midifile.read(); //
                                    uint8_t bb =_midifile.read();
                                    uint8_t denominator = 2^dd;
                                    _track_position[i]+=4;
                                    Serial.printf("Time Signature: %d / %d, %d %d\n", nn, denominator, cc, bb);
                                }
                                break;
                            }

                            case 0x59: {
                                // FF 59 02 sf mi Key Signature
                                char c = _midifile.read();
                                _track_position[i]++;
                                if (c == 2) {
                                    // nn dd cc bb
                                    uint8_t sf =_midifile.read();
                                    uint8_t mi =_midifile.read();

                                    _track_position[i]+=2;
                                    Serial.printf("Key Signature: %d %d\n", sf, mi);
                                }
                                break;
                            }

                            default: {
                                if ((1 <= nextByte) && (nextByte <= 0x0F))
                                    readMetaText(i);
                                else {
                                    uint8_t nn =_midifile.read();
                                    _track_position[i]++;

                                    for (uint8_t jjj=0; jjj<nn; jjj++) {
                                        uint8_t xx =_midifile.read();
                                        _track_position[i]++;
                                    }

                                    Serial.printf("Meta - 0x%x 0x%x 0x%02x\n", status_byte, nextByte, nn);
                                }
                                break;
                            }

                        }
                        break;
                    }

                    default: {
                        Serial.printf("unread - status_byte %x %d\n", status_byte, status_byte);
                        break;
                    }

                }

            }
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
    Serial.printf("text: ");
    Serial.print(text);
    Serial.println();
    delete[] text;
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

const char* MidiReader::voice_message_status_name(unsigned char status) {
    switch (status & 0xF0) {
        case 0x80 : return "note off";
        case 0x90 : return "note on";
        case 0xA0 : return "after touch";
        case 0xB0 : return "control change";
        case 0xC0 : return "program change";
        case 0xD0 : return "channel pressure";
        case 0xE0 : return "pitch wheel";
    }
    return "[?]";
}
