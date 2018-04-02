//
// Created by Nicholas Newdigate on 17/03/2018.
//

#ifndef ARDUINO_ABSTRACTION_TFTCOLORHELPER_H
#define ARDUINO_ABSTRACTION_TFTCOLORHELPER_H

#include <cstdint>

class TFTColorHelper {
public:
    static uint16_t ConvertRGBto565(uint8_t r, uint8_t g, uint8_t b);
    static uint32_t Convert565toRGB(uint16_t color);
};

#endif //ARDUINO_ABSTRACTION_TFTCOLORHELPER_H
