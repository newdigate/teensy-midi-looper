//
// Created by Nicholas Newdigate on 07/04/2018.
//

#include "SequencerTrack.h"

void SequencerTrack::update(unsigned long millis) {
    if (_epoch == 0) {
        _epoch = millis;
        _loop_phase = 0;
    } else {
        int deltaMillis = millis - _last_tick_milliseconds;
        if (deltaMillis >= _milliseconds_per_tick) {
            unsigned int deltaTicks = deltaMillis / _milliseconds_per_tick;
            tick(deltaTicks);
            _last_tick_milliseconds = millis;
        }
    }
}

void SequencerTrack::tick(unsigned int delta_ticks) {
    uint8_t old_position_ticks = _position_ticks;
    _position_ticks += delta_ticks;
    uint8_t delta_loop_phase = delta_ticks / _durationTicks;
    if (delta_loop_phase > 0) {
        _loop_phase += delta_loop_phase;
    }
}
