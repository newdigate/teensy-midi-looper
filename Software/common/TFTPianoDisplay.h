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

    bool _shouldUpdatePiano, _forceFullKeyboardRedraw  = true;
    byte _keysWhichArePressed[22];
    byte _oldkeysWhichArePressed[22];

    bool isKeyPressed(byte key);
    bool wasKeyPressed(byte key);
    void setWasKeyPressed(byte key, bool value);
    bool isAnyKeyPressed(byte key);

    const int _sizeFactor = 5;
    int _whiteKeyWidth = (35*10)/_sizeFactor;
    int _blackKeyWidth1 = (21*10)/_sizeFactor;
    int _blackKeyWidth2 = (20*10)/_sizeFactor;
    int _two_thirds_key_height = (70*2/3)/_sizeFactor;
    int _one_thirds_key_height = (70*1/3)/_sizeFactor;
    int _key_offset_c = 0;
    int _key_offset_c_sharp = (_blackKeyWidth1)/10;
    int _key_offset_c_sharp_end = (2*_blackKeyWidth1)/10;
    int _key_offset_d = (_whiteKeyWidth)/10;
    int _key_offset_d_sharp = (_blackKeyWidth1*3)/10;
    int _key_offset_d_sharp_end = (4*_blackKeyWidth1)/10;
    int _key_offset_e = (_whiteKeyWidth*2)/10;
    int _key_offset_f = (_whiteKeyWidth*3)/10;
    int _key_offset_f_sharp = _key_offset_f + (_blackKeyWidth2)/10;
    int _key_offset_f_sharp_end = _key_offset_f_sharp + (_blackKeyWidth2)/10;
    int _key_offset_g = (_whiteKeyWidth*4)/10;
    int _key_offset_g_sharp = _key_offset_f + (_blackKeyWidth2*3)/10;
    int _key_offset_g_sharp_end = _key_offset_g_sharp + (_blackKeyWidth2)/10;
    int _key_offset_a = (_whiteKeyWidth*5)/10;
    int _key_offset_a_sharp = _key_offset_f + (_blackKeyWidth2*5)/10;
    int _key_offset_a_sharp_end = _key_offset_a_sharp + (_blackKeyWidth2)/10;
    int _key_offset_b = (_whiteKeyWidth*6)/10;
    int _key_offset_b_end = ((_whiteKeyWidth*7)/10) - 1;
};

#endif
