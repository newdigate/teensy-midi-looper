//
// Created by Nicholas Newdigate on 07/04/2018.
//

#ifndef TEENSY_MIDI_COMMON_SEQUENCERTRACK_H
#define TEENSY_MIDI_COMMON_SEQUENCERTRACK_H

#include <cstdint>
#include "Tempo.h"
#include "../Delegate.h"

class SequencerTrack {
public:
    SequencerTrack(Tempo &tempo) :
        _tempo(tempo),
        onPhaseChanged()
    {
        _milliseconds_per_tick = _tempo.milliseconds_per_tick();
    }

    void update(unsigned long millis);
    uint8_t _loop_phase = 0;
    Delegate<const SequencerTrack&> onPhaseChanged;

private:
    unsigned long _epoch = 0;

    Tempo &_tempo;
    uint8_t _durationBars = 1;
    uint16_t _position_ticks = 0, _durationTicks = 64 * _durationBars * 4;
    unsigned int _milliseconds_per_tick = 0;
    unsigned long _last_tick_milliseconds = 0;

    void tick(unsigned int delta_ticks);
};


#endif //TEENSY_MIDI_COMMON_SEQUENCERTRACK_H
