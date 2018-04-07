//
// Created by Nicholas Newdigate on 17/03/2018.
//

#include "TFTSongPositionIndicator.h"

void TFTSongPositionIndicator::update(unsigned long millis) {
}

void TFTSongPositionIndicator::setSongPosition(SongPosition position) {
    if (_currentPosition.bar !=  position.bar) {

        _tft->setCursor(_x, _y);
        sprintf(_char_buf, "%04d:", _currentPosition.bar);
        _tft->setTextColor(_indicatorOffColor);
        _tft->print(_char_buf);

        _currentPosition.bar = position.bar;

        _tft->setCursor(_x, _y);
        sprintf(_char_buf, "%04d:", _currentPosition.bar);
        _tft->setTextColor(_indicatorOnColor);
        _tft->print(_char_buf);
    }

    if ( _currentPosition.beat != position.beat) {
        uint8_t offset = (6*5);
        _tft->setCursor(_x+offset, _y);
        sprintf(_char_buf, "%d", _currentPosition.beat);
        _tft->setTextColor(_indicatorOffColor);
        _tft->print(_char_buf);

        _currentPosition.beat = position.beat;

        _tft->setCursor(_x+offset, _y);
        sprintf(_char_buf, "%d", _currentPosition.beat);
        _tft->setTextColor(_indicatorOnColor);
        _tft->print(_char_buf);
    }

}
