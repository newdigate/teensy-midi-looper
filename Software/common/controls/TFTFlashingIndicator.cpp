//
// Created by Nicholas Newdigate on 17/03/2018.
//

#include "TFTFlashingIndicator.h"

void TFTFlashingIndicator::update(unsigned long millis) {
    if ( _indicatorEnabled && (_last_millis == 0 || millis - _last_millis > _half_period)) {
        _last_millis = millis;
        _indicatorLEDState = !_indicatorLEDState;
        _tft->fillCircle(8,64+4, 4,_indicatorLEDState? _indicatorOnColor : _indicatorOffColor);
    }
}
