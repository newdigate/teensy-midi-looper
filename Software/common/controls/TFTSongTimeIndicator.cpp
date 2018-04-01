//
// Created by Nicholas Newdigate on 01/04/2018.
//
const char *minutefmtString = "%02d:";
const char *hundredsfmtString = "%d";
const char *secondfmtString = "%02d.";
const char *hoursfmtString = "%02d:";

#include "TFTSongTimeIndicator.h"
void TFTSongTimeIndicator::update(unsigned long millis) {

    uint32_t hours = (millis / (1000 * 60 * 60)) % 24;
    uint32_t minutes = (millis / (1000 * 60)) % 60;
    uint32_t seconds = (millis / 1000) % 60;
    uint32_t hundreds = (millis / 100) % 10;
    if (hours != _hours ) {

        _tft->setCursor(_x, _y);
        char *c = new char[6];
        sprintf(c, hoursfmtString, _hours);
        _tft->setTextColor(_indicatorOffColor);
        _tft->print(c);

        _hours = hours;

        _tft->setCursor(_x, _y);
        sprintf(c, hoursfmtString, _hours);
        _tft->setTextColor(_indicatorOnColor);
        _tft->print(c);
    }

    if (minutes != _minutes ) {

        uint8_t offset = (3 * 6);
        _tft->setCursor(_x+offset, _y);
        char *c = new char[6];
        sprintf(c, minutefmtString, _minutes);
        _tft->setTextColor(_indicatorOffColor);
        _tft->print(c);

        _minutes = minutes;

        _tft->setCursor(_x+offset, _y);
        sprintf(c, minutefmtString, _minutes);
        _tft->setTextColor(_indicatorOnColor);
        _tft->print(c);
    }

    if (seconds != _seconds ) {
        uint8_t offset = (6 * 6);
        _tft->setCursor(_x+offset, _y);
        char *c = new char[6];
        sprintf(c, secondfmtString, _seconds);
        _tft->setTextColor(_indicatorOffColor);
        _tft->print(c);

        _seconds = seconds;

        _tft->setCursor(_x+offset, _y);
        sprintf(c, secondfmtString, _seconds);
        _tft->setTextColor(_indicatorOnColor);
        _tft->print(c);
    }

    if (hundreds != _hundreds) {

        uint8_t offset = (9 * 6);

        _tft->setCursor(_x+offset, _y);
        char *c = new char[6];
        sprintf(c, hundredsfmtString, _hundreds);
        _tft->setTextColor(_indicatorOffColor);
        _tft->print(c);

        _hundreds = hundreds;

        _tft->setCursor(_x+offset, _y);
        sprintf(c, hundredsfmtString, _hundreds);
        _tft->setTextColor(_indicatorOnColor);
        _tft->print(c);
    }
}
