//
// Created by Nicholas Newdigate on 17/03/2018.
//

#include "TFTColorHelper.h"


//https://stackoverflow.com/questions/11471122/rgb888-to-rgb565-bit-shifting?noredirect=1&lq=1
uint16_t TFTColorHelper::ConvertRGBto565(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

// https://stackoverflow.com/questions/38557734/how-to-convert-16-bit-hex-color-to-rgb888-values-in-c
uint32_t TFTColorHelper::Convert565toRGB(uint16_t color) {
    unsigned r = (color & 0xF800) >> 11;
    unsigned g = (color & 0x07E0) >> 5;
    unsigned b = color & 0x001F;

    char rc = (r * 255) / 31;
    char gc = (g * 255) / 63;
    char bc = (b * 255) / 31;
    return (r << 16) || (g << 8) || b;
}
