//
// Created by Nicholas Newdigate on 07/04/2018.
//

#ifndef TEENSY_MIDI_COMMON_TFTLOOPINDICATOR_H
#define TEENSY_MIDI_COMMON_TFTLOOPINDICATOR_H

#if ARDUINO >= 100
#include <Adafruit_GFX.h>    // Core graphics library
#else
#include "Adafruit/Adafruit_GFX.h"    // Core graphics library
#endif

#include "../utils/TFTColorHelper.h"
#include "../sequencer/SequencerTrack.h"

class TFTLoopIndicator {
public :
    inline TFTLoopIndicator(Adafruit_GFX *tft, SequencerTrack &track, int x, int y) : _track(track) {
        _tft = tft;
        _x = x;
        _y = y;
        _width = 16;
        _height = 16;
        _numBars = 1;
        _oldPosition = 0;
        _currentPosition = 0;
        _maxPosition = 64*_numBars*4;
        _lastSegment = 0;
        _track.onPhaseChanged += [&] (const SequencerTrack& track) {
            //Serial.printf(">>%x\n", (unsigned long)this);
            this->phaseChanged();
        };
    }

    void update(unsigned long millis);
    
    inline void phaseChanged() {
        //Serial.printf("::%x\n", (unsigned long)this);
        _needs_refresh = true;
    }
    volatile bool _needs_refresh = true;

private:
    uint16_t _indicatorOnColor = TFTColorHelper::ConvertRGBto565(255,255,0);
    uint16_t _indicatorOffColor = TFTColorHelper::ConvertRGBto565(0,0,0);
    Adafruit_GFX *_tft;
    int _x, _y, _width, _height, _numBars, _currentPosition, _oldPosition, _maxPosition;
    SequencerTrack &_track;
    uint16_t _lastSegment;
    int fillArc2(int x, int y, int start_angle, int seg_count, int rx, int ry, int w, unsigned int colour);
};


#endif //TEENSY_MIDI_COMMON_TFTLOOPINDICATOR_H
