//
// Created by Nicholas Newdigate on 01/04/2018.
//
const char *minutefmtString = "%02d:";
const char *hundredsfmtString = "%d";
const char *secondfmtString = "%02d.";
const char *hoursfmtString = "%02d:";

#include "TFTSongTimeIndicator.h"

//char TFTSongTimeIndicator::_char_buf[] = "        ";
void TFTSongTimeIndicator::update(unsigned long millis) {

    uint32_t hours = (millis / (1000 * 60 * 60)) % 24;
    uint32_t minutes = (millis / (1000 * 60)) % 60;
    uint32_t seconds = (millis / 1000) % 60;
    uint32_t hundreds = (millis / 100) % 10;
    if (hours != _hours ) {

        _tft->setCursor(_x, _y);
        sprintf(_char_buf, hoursfmtString, _hours);
        _tft->setTextColor(_indicatorOffColor);
        _tft->print(_char_buf);

        _hours = hours;

        _tft->setCursor(_x, _y);
        sprintf(_char_buf, hoursfmtString, _hours);
        _tft->setTextColor(_indicatorOnColor);
        _tft->print(_char_buf);
    }

    if (minutes != _minutes ) {

        uint8_t offset = (3 * 6);
        _tft->setCursor(_x+offset, _y);
        sprintf(_char_buf, minutefmtString, _minutes);
        _tft->setTextColor(_indicatorOffColor);
        _tft->print(_char_buf);

        _minutes = minutes;

        _tft->setCursor(_x+offset, _y);
        sprintf(_char_buf, minutefmtString, _minutes);
        _tft->setTextColor(_indicatorOnColor);
        _tft->print(_char_buf);
    }

    if (seconds != _seconds ) {
        uint8_t offset = (6 * 6);
        _tft->setCursor(_x+offset, _y);
        sprintf(_char_buf, secondfmtString, _seconds);
        _tft->setTextColor(_indicatorOffColor);
        _tft->print(_char_buf);

        _seconds = seconds;

        _tft->setCursor(_x+offset, _y);
        sprintf(_char_buf, secondfmtString, _seconds);
        _tft->setTextColor(_indicatorOnColor);
        _tft->print(_char_buf);
    }

    if (hundreds != _hundreds) {

        uint8_t offset = (9 * 6);

        _tft->setCursor(_x+offset, _y);
        sprintf(_char_buf, hundredsfmtString, _hundreds);
        _tft->setTextColor(_indicatorOffColor);
        _tft->print(_char_buf);

        _hundreds = hundreds;

        _tft->setCursor(_x+offset, _y);
        sprintf(_char_buf, hundredsfmtString, _hundreds);
        _tft->setTextColor(_indicatorOnColor);
        _tft->print(_char_buf);
    }
}
