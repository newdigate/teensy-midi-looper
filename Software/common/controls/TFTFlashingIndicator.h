//
// Created by Nicholas Newdigate on 17/03/2018.
//

#ifndef ARDUINO_ABSTRACTION_TFTFLASHINGINDICATOR_H
#define ARDUINO_ABSTRACTION_TFTFLASHINGINDICATOR_H
#include <Adafruit/Adafruit_GFX.h>
#include <Adafruit/TFTColorHelper.h>
class TFTFlashingIndicator {
public:
    inline TFTFlashingIndicator(Adafruit_GFX *tft, int x, int y) {
        _tft = tft;
        _x = x;
        _y = y;
    }

    void update(unsigned long millis);


private:
    bool _indicatorEnabled = true;
    bool _indicatorLEDState = true;
    unsigned long _half_period = 500; // half the duration of the entire blink cycle
    unsigned long _last_millis = 0;
    uint16_t _indicatorOnColor = TFTColorHelper::ConvertRGBto565(255,0,0);
    uint16_t _indicatorOffColor = 0;
    Adafruit_GFX *_tft;
    int _x, _y;
};


#endif //ARDUINO_ABSTRACTION_TFTFLASHINGINDICATOR_H
