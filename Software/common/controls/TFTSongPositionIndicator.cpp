//
// Created by Nicholas Newdigate on 17/03/2018.
//

#include "TFTSongPositionIndicator.h"

void TFTSongPositionIndicator::update(unsigned long millis) {
}

void TFTSongPositionIndicator::setSongPosition(SongPosition position) {
    if (_currentPosition.bar !=  position.bar || _currentPosition.beat != position.beat) {

        _tft->setCursor(_x/8, _y/8);
        char *c = new char[12];
        sprintf(c, "%i : %i", _currentPosition.bar, _currentPosition.beat);
        _tft->setTextColor(_indicatorOffColor);
        _tft->print(c);

        _currentPosition = position;

        _tft->setCursor(_x/8, _y/8);
        sprintf(c, "%i : %i", _currentPosition.bar, _currentPosition.beat);
        _tft->setTextColor(_indicatorOnColor);
        _tft->print(c);
    }
}
