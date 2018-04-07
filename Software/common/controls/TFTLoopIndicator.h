//
// Created by Nicholas Newdigate on 07/04/2018.
//

#ifndef TEENSY_MIDI_COMMON_TFTLOOPINDICATOR_H
#define TEENSY_MIDI_COMMON_TFTLOOPINDICATOR_H


#include <Adafruit/Adafruit_GFX.h>
#include "../utils/TFTColorHelper.h"

class TFTLoopIndicator {
public :
    inline TFTLoopIndicator(Adafruit_GFX *tft, int x, int y) {
        _tft = tft;
        _x = x;
        _y = y;
        _numBars = 16;
        _oldPosition = 0;
        _currentPosition = 0;
        _maxPosition = 64;
    }

    void update(unsigned long millis);

private:
    uint16_t _indicatorOnColor = TFTColorHelper::ConvertRGBto565(255,0,0);
    uint16_t _indicatorOffColor = 0;
    Adafruit_GFX *_tft;
    int _x, _y, _numBars, _currentPosition, _oldPosition, _maxPosition;
};


#endif //TEENSY_MIDI_COMMON_TFTLOOPINDICATOR_H
