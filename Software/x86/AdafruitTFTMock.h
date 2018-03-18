//
// Created by Nicholas Newdigate on 11/03/2018.
//

#ifndef ARDUINO_MIDI_WRITER_ADAFRUITTFTMOCK_H
#define ARDUINO_MIDI_WRITER_ADAFRUITTFTMOCK_H



#include <Adafruit/Adafruit_GFX.h>
#include <iostream>
#include <cstring>

const std::string red("\033[0;31m");
const std::string green("\033[1;32m");
const std::string yellow("\033[1;33m");
const std::string cyan("\033[0;36m");
const std::string magenta("\033[0;35m");
const std::string reset("\033[0m");


class AdafruitTFTMock : public Adafruit_GFX{
public:
    AdafruitTFTMock(int16_t w, int16_t h);
    inline int write(uint8_t u) {
        return 0;
    }

    inline int write(unsigned char const*, unsigned long) {
        return 0;
    }

    inline void drawPixel(int16_t x, int16_t y, uint16_t color) {

    }

    inline void startWrite(void) {

    }
    inline void writePixel(int16_t x, int16_t y, uint16_t color) {
        print("\033[");
        print((byte)y);
        print(";");
        print((byte)y);
        print("H");
        if (color > 0) {
            std::cout << "*";
        }
    }
    inline void writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {}
    inline void writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {}
    inline void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {}
    inline void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {}
    inline void endWrite(void) {}

    // CONTROL API
    // These MAY be overridden by the subclass to provide device-specific
    // optimized code.  Otherwise 'generic' versions are used.
    inline void setRotation(uint8_t r) {}
    inline void invertDisplay(bool i) {}

    // BASIC DRAW API
    // These MAY be overridden by the subclass to provide device-specific
    // optimized code.  Otherwise 'generic' versions are used.
    inline void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {}
    inline void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {}
    inline void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {}
    inline void fillScreen(uint16_t color) {}
    // Optional and probably not necessary to change
    inline void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {}
    inline void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {}


};


#endif //ARDUINO_MIDI_WRITER_ADAFRUITTFTMOCK_H
