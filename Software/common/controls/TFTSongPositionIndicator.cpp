//
// Created by Nicholas Newdigate on 17/03/2018.
//

#include "TFTSongPositionIndicator.h"

void TFTSongPositionIndicator::update(unsigned long millis) {
}

void TFTSongPositionIndicator::setSongPosition(SongPosition position) {
    if (_currentPosition.bar !=  position.bar) {

        _tft->setCursor(_x, _y);
        char *c = new char[5];
        sprintf(c, "%04d:", _currentPosition.bar);
        _tft->setTextColor(_indicatorOffColor);
        _tft->print(c);

        _currentPosition.bar = position.bar;

        _tft->setCursor(_x, _y);
        sprintf(c, "%04d:", _currentPosition.bar);
        _tft->setTextColor(_indicatorOnColor);
        _tft->print(c);
    }

    if ( _currentPosition.beat != position.beat) {
        uint8_t offset = (6*5);
        _tft->setCursor(_x+offset, _y);
        char *c = new char[5];
        sprintf(c, "%d", _currentPosition.beat);
        _tft->setTextColor(_indicatorOffColor);
        _tft->print(c);

        _currentPosition.beat = position.beat;

        _tft->setCursor(_x+offset, _y);
        sprintf(c, "%d", _currentPosition.beat);
        _tft->setTextColor(_indicatorOnColor);
        _tft->print(c);
    }

}
