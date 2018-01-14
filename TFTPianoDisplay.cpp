#include "Arduino.h"
#include "TFTPianoDisplay.h"

TFTPianoDisplay::TFTPianoDisplay(Adafruit_GFX &tft, unsigned int octaves, unsigned int startOctave){
    _tft = &tft;
    _x = 0;
    _y = 0;
    _height = 32;
    _width = 128;
    _octaves = octaves;
    _startOctave = startOctave; 
    
    _shouldUpdatePiano = true;
    
    for (unsigned int i=0; i < sizeof(_keysWhichArePressed); i++) {
      _keysWhichArePressed[i] = 0x00;
    }
}

bool TFTPianoDisplay::displayNeedsUpdating() {
  return _shouldUpdatePiano;
}

void TFTPianoDisplay::setPosition(byte x, byte y) {
    _x = x;
    _y = y;
}

void TFTPianoDisplay::keyDown(byte key) {
  byte byteNumberOfKey = key / 8;
  byte bitNumberOfKey = key % 8;
  
  //Serial.printf("ON: %d,%d ON\n", byteNumberOfKey, bitNumberOfKey);
  byte b = _keysWhichArePressed[byteNumberOfKey];
  //Serial.printf("before ON: %d,%d ::%x\n", byteNumberOfKey, bitNumberOfKey, b);
  bitSet( b, bitNumberOfKey);
  //Serial.printf("ON: %d,%d ::%x\n", byteNumberOfKey, bitNumberOfKey, b);
  _keysWhichArePressed[byteNumberOfKey] = b;
  _shouldUpdatePiano = true;
}

void TFTPianoDisplay::keyUp(byte key) {
  byte byteNumberOfKey = key / 8;
  byte bitNumberOfKey = key % 8;
  byte b = _keysWhichArePressed[byteNumberOfKey];
  //Serial.printf("before OFF: %d,%d ::%x\n", byteNumberOfKey, bitNumberOfKey, b);
  bitClear( b, bitNumberOfKey);
  _keysWhichArePressed[byteNumberOfKey] = b;
  //Serial.printf("OFF: %d,%d ::%x\n", byteNumberOfKey, bitNumberOfKey, b);
  _shouldUpdatePiano = true;
}

bool TFTPianoDisplay::isKeyPressed(byte key) {
    byte byteNumberOfKey = key / 8;
    byte bitNumberOfKey = key % 8;
    return bitRead( _keysWhichArePressed[byteNumberOfKey], bitNumberOfKey);
}

bool TFTPianoDisplay::isAnyKeyPressed(byte key) {
    byte index = key % 24;
    for (int i = 0; i < 3; i++) {
      byte transformedKey = index + (i * 24);
      byte byteNumberOfKey = transformedKey / 8;
      byte bitNumberOfKey = transformedKey % 8;
      bool keydown = bitRead( _keysWhichArePressed[byteNumberOfKey], bitNumberOfKey);
      if (keydown) return true;
    }
    return false;
}

void TFTPianoDisplay::drawPiano() {
  /*
    for (int i=0; i< 14; i++) {
      char c[] = "         ";
      itoa(keysWhichArePressed[i],c,2);
      Serial.printf("%d: %s\n", i, c);
    }
    Serial.println();
    */
    const byte b[] = {/* E*/ 4 + 12, /* D */2 + 12, /* C */ 0 + 12, /* B */ 11, /* A */ 9, /* G */ 7, /*F*/ 5 };
    //Draw the white keys on the keyboard
    for (int i=0; i<24; i++) {
      byte mod = i % 7;
      byte key = b[mod];
      bool isDown = isAnyKeyPressed(key);

      /*if (isDown)
        Serial.printf("[i=%d, i%%7 = %d, b[i%%7]=%d, DOWN]\n", i, mod, key);
      else
        Serial.printf("[i=%d, i%%7 = %d, b[i%%7]=%d, UP]\n", i, mod, key);
*/
      
      if (isDown) 
        _tft->fillRect(0,i*5,32,4,ST7735_BLUE);
      else 
        _tft->fillRect(0,i*5,32,4,ST7735_WHITE);     
    }
    //Draw the black keys on the keyboard
    _tft->fillRect(0,3,16,3, isAnyKeyPressed(3)? ST7735_BLUE : ST7735_BLACK);
    _tft->fillRect(0,8,16,3, isAnyKeyPressed(1)? ST7735_BLUE : ST7735_BLACK);
    
    _tft->fillRect(0,18,16,3, isAnyKeyPressed(10)? ST7735_BLUE : ST7735_BLACK);
    _tft->fillRect(0,23,16,3, isAnyKeyPressed(9)? ST7735_BLUE : ST7735_BLACK);
    _tft->fillRect(0,28,16,3, isAnyKeyPressed(7)? ST7735_BLUE : ST7735_BLACK);
    
    _tft->fillRect(0,38,16,3, isAnyKeyPressed(3)? ST7735_BLUE : ST7735_BLACK);
    _tft->fillRect(0,43,16,3, isAnyKeyPressed(1)? ST7735_BLUE : ST7735_BLACK);
    
    _tft->fillRect(0,53,16,3, isAnyKeyPressed(10)? ST7735_BLUE : ST7735_BLACK);
    _tft->fillRect(0,58,16,3, isAnyKeyPressed(9)? ST7735_BLUE : ST7735_BLACK);
    _tft->fillRect(0,63,16,3, isAnyKeyPressed(7)? ST7735_BLUE : ST7735_BLACK);
    
    _tft->fillRect(0,73,16,3, isAnyKeyPressed(3)? ST7735_BLUE : ST7735_BLACK);
    _tft->fillRect(0,78,16,3, isAnyKeyPressed(1)? ST7735_BLUE : ST7735_BLACK);
    
    _tft->fillRect(0,88,16,3, isAnyKeyPressed(10)? ST7735_BLUE : ST7735_BLACK);
    _tft->fillRect(0,93,16,3, isAnyKeyPressed(9)? ST7735_BLUE : ST7735_BLACK);
    _tft->fillRect(0,98,16,3, isAnyKeyPressed(7)? ST7735_BLUE : ST7735_BLACK);
    
    _tft->fillRect(0,108,16,3, isAnyKeyPressed(3)? ST7735_BLUE : ST7735_BLACK);
    _tft->fillRect(0,113,16,3, isAnyKeyPressed(1)? ST7735_BLUE : ST7735_BLACK);
    _shouldUpdatePiano = false;
}
