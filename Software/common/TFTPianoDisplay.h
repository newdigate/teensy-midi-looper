#ifndef TFTPianoDisplay_h
#define TFTPianoDisplay_h

#if ARDUINO >= 100
#include "Arduino.h"
#include <Adafruit_GFX.h>    // Core graphics library
#else
#include "mock_arduino.h"
#include "Adafruit/Adafruit_GFX.h"    // Core graphics library
#endif

class TFTPianoDisplay
{
public:
    TFTPianoDisplay(Adafruit_GFX &tft, byte octaves, byte startOctave, byte x, byte y);
    void setPosition(byte x, byte y);
    void keyDown(byte key);
    void keyUp(byte key);
    void drawPiano();
    bool displayNeedsUpdating();

private:
    Adafruit_GFX *_tft;
    byte _x, _y;
    byte _height, _width;
    byte _offsetKeyZero;
    unsigned int _octaves, _startOctave;

    bool _shouldUpdatePiano = true;
    byte _keysWhichArePressed[22];

    bool isKeyPressed(byte key);
    bool isAnyKeyPressed(byte key);
};

#endif
