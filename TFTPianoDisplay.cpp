#include "Arduino.h"
#include "TFTPianoDisplay.h"

TFTPianoDisplay::TFTPianoDisplay(Adafruit_GFX &tft, byte octaves, byte startOctave, byte x, byte y){
    _tft = &tft;
    _x = x;
    _y = y;
    _height = 32;
    _width = 128;
    _octaves = octaves;
    _startOctave = startOctave; 
    _offsetKeyZero = ( 12 * _startOctave ); 
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
  if (key < _offsetKeyZero) return;
  if (key >= _offsetKeyZero+(_octaves*12)) return;
  
  byte index = key - _offsetKeyZero;
  
  byte byteNumberOfKey = index / 8;
  byte bitNumberOfKey = index % 8;
  
  //Serial.printf("ON: %d,%d -%d\n", byteNumberOfKey, bitNumberOfKey, key);

  //Serial.printf("before ON: %d,%d ::%x\n", byteNumberOfKey, bitNumberOfKey, b);
  bitSet( _keysWhichArePressed[byteNumberOfKey], bitNumberOfKey);
  //Serial.printf("ON: %d,%d ::%x\n", byteNumberOfKey, bitNumberOfKey, b);
  //_keysWhichArePressed[byteNumberOfKey] = b;
  _shouldUpdatePiano = true;
}

void TFTPianoDisplay::keyUp(byte key) {
  if (key < _offsetKeyZero) return;
  if (key >= _offsetKeyZero+(_octaves*12)) return;
  
  byte index = key - _offsetKeyZero;
  byte byteNumberOfKey = index / 8;
  byte bitNumberOfKey = index % 8;

  //Serial.printf("before OFF: %d,%d ::%x\n", byteNumberOfKey, bitNumberOfKey, b);
  bitClear( _keysWhichArePressed[byteNumberOfKey], bitNumberOfKey);

  Serial.printf("OFF: %d,%d ::%x\n", byteNumberOfKey, bitNumberOfKey, _keysWhichArePressed[byteNumberOfKey]);
  _shouldUpdatePiano = true;
}

bool TFTPianoDisplay::isKeyPressed(byte key) {
    if (key < _offsetKeyZero) return false;
    if (key >= _offsetKeyZero+(_octaves*12)) return false;
    
    byte index = key - _offsetKeyZero;
    byte byteNumberOfKey = index / 8;
    byte bitNumberOfKey = index % 8;
    return bitRead( _keysWhichArePressed[byteNumberOfKey], bitNumberOfKey);
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
    const byte b[] = {/* C */ 0, /* D */ 2, /* E */ 4, /* F */ 5, /* G */ 7, /* A */ 9, /* B */ 11 };
    //Draw the white keys on the keyboard
    for (unsigned int octave=0; octave < _octaves; octave++) { 
      for (int i=0; i<7; i++) {
        byte key = b[i] + (octave * 12) + _offsetKeyZero;
        bool isDown = isKeyPressed(key);
        //Serial.printf("(%d) ", key);
        /*if (isDown)
          Serial.printf("[i=%d, i%%7 = %d, b[i%%7]=%d, DOWN]\n", i, mod, key);
        else
          Serial.printf("[i=%d, i%%7 = %d, b[i%%7]=%d, UP]\n", i, mod, key);
  */
        
        if (isDown) 
          _tft->fillRect(_x + i*5 + (octave*5*7), _y, 4, 32, ST7735_BLUE);
        else 
          _tft->fillRect(_x + i*5 + (octave*5*7), _y, 4, 32, ST7735_WHITE);     
      }
      Serial.printf("\n");
      const byte blackKeys[] = {/* C# */ 1, /* D# */ 3, /*Skip*/ 0, /* F# */ 6, /* G# */ 8, /* A# */ 10, /*Skip*/ 0};
      
      //Draw the black keys on the keyboard
      for (int i=0; i<6; i++){
        if ( i==2 ) continue;
        byte key = blackKeys[i] + (octave * 12) + _offsetKeyZero;
        _tft->fillRect( _x+3 + (5*i) + (octave*5*7), _y, 3, 16, isKeyPressed(key)? ST7735_BLUE : ST7735_BLACK);
      }
    }
    _shouldUpdatePiano = false;
}
