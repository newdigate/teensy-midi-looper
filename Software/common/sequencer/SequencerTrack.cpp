//
// Created by Nicholas Newdigate on 07/04/2018.
//

#include "SequencerTrack.h"

void SequencerTrack::update(unsigned long millis) {

        int deltaMillis = millis - _last_tick_milliseconds;
        if (deltaMillis >= _milliseconds_per_tick) {
            unsigned int deltaTicks = deltaMillis / _milliseconds_per_tick;
            tick(deltaTicks);
            _last_tick_milliseconds = millis;
        }

}

void SequencerTrack::tick(unsigned int delta_ticks) {
    _position_ticks += delta_ticks;
    if (_position_ticks > _durationTicks)
        _position_ticks %= _durationTicks;

    uint8_t delta_loop_phase = delta_ticks * 256/ _durationTicks;
    if (delta_loop_phase > 0) {
        _loop_phase += delta_loop_phase;
        onPhaseChanged(*this);
    }
}
