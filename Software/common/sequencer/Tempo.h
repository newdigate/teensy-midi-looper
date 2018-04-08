//
// Created by Nicholas Newdigate on 07/04/2018.
//

#ifndef TEENSY_MIDI_COMMON_TEMPO_H
#define TEENSY_MIDI_COMMON_TEMPO_H

struct Tempo {
    Tempo(int beats_per_minute);
    Tempo();

    int _beats_per_minute = 120;

    unsigned int milliseconds_per_tick() {
        float seconds_per_beat = 60.0 / _beats_per_minute;
        int millis_per_beat = seconds_per_beat * 1000.0;
        int millis_per_tick = millis_per_beat / 64;
        return millis_per_tick;
    }
};

#endif //TEENSY_MIDI_COMMON_TEMPO_H
