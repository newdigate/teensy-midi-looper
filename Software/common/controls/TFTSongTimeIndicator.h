//
// Created by Nicholas Newdigate on 01/04/2018.
//

#ifndef TEENSY_MIDI_COMMON_TFTSONGTIMEINDICATOR_H
#define TEENSY_MIDI_COMMON_TFTSONGTIMEINDICATOR_H


#include <cstdint>
#include <Adafruit/Adafruit_GFX.h>
#include <Adafruit/TFTColorHelper.h>

class TFTSongTimeIndicator {
public:
    inline TFTSongTimeIndicator(Adafruit_GFX *tft, int x, int y) {
        _tft = tft;
        _x = x;
        _y = y;
    }

    void update(unsigned long millis);


private:
    int32_t _hours, _minutes, _seconds, _hundreds = 0;
    uint16_t _indicatorOnColor = TFTColorHelper::ConvertRGBto565(0,0,255);
    uint16_t _indicatorOffColor = 0;
    Adafruit_GFX *_tft;
    int _x, _y;
};


#endif //TEENSY_MIDI_COMMON_TFTSONGTIMEINDICATOR_H
