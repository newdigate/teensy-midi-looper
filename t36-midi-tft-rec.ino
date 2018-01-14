#include <MIDI.h>
#include <cppQueue.h>
#include "MidiWriter.h"

// This Teensy3 native optimized version requires specific pins
//
#define sclk 13  // SCLK can also use pin 14
#define mosi 11  // MOSI can also use pin 7
#define cs   10  // CS & DC can use pins 2, 6, 9, 10, 15, 20, 21, 22, 23
#define dc   9   //  but certain pairs must NOT be used: 2+10, 6+9, 20+23, 21+22
#define rst  8   // RST can use any pin
#define sdcs 4   // CS for SD card, can use any pin

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

// Teensy 3.5 & 3.6 on-board: BUILTIN_SDCARD
// Wiz820+SD board: pin 4
// Teensy 2.0: pin 0
// Teensy++ 2.0: pin 20
const int chipSelect = BUILTIN_SDCARD;


Adafruit_ST7735 tft = Adafruit_ST7735(cs, dc, rst);

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1,     midiA);

typedef struct strRec {
  unsigned long time;
  uint8_t  entry1;
  uint8_t  entry2;
  uint8_t  entry3;
  uint8_t  entry4;
} Rec;

//Queue q(sizeof(Rec), 100, FIFO);
MidiWriter midi_writer;

void drawPiano();
byte keysWhichArePressed[22];

void setup()
{
  tft.initR(INITR_GREENTAB); // initialize a ST7735R chip, green tab
  tft.setRotation(1);
  tft.setTextWrap(true);
  tft.fillScreen(ST7735_BLACK);
  //while (!Serial) {
  //  delay(100);
  //}
    Serial.begin(9600);
    Serial.println("hello!");
    
    // Initiate MIDI communications, listen to all channels
    midiA.begin(MIDI_CHANNEL_OMNI);
    //Serial.println("midi has begun!");



   //Serial.print("Initializing SD card...");
  
    // see if the card is present and can be initialized:
    if (!SD.begin(chipSelect)) {
      //Serial.println("Card failed, or not present");
      // don't do anything more:
      return;
    }
    //Serial.println("card initialized.");
    midi_writer.setFilename("af");
    midi_writer.writeHeader();
    midi_writer.flush();

    for (unsigned int i=0; i < sizeof(keysWhichArePressed); i++) {
      keysWhichArePressed[i] = 0x00;
    }
}

unsigned long previousSixtyFourth=0;

const float beats_per_minute = 120.0;
const float beats_per_second = beats_per_minute / 60;
const float seconds_per_beat = 60 / beats_per_minute;
const float millis_per_beat = seconds_per_beat * 1000;
const float millis_per_64th = millis_per_beat/64;
const float millis_per_256th = millis_per_beat/256;


int beat, lastbeat, bar, lastbar;

bool firstNote = true;

unsigned long previous = 0;

unsigned long lastDisplayUpdate = 0;
bool shouldUpdatePiano = true;

bool isKeyPressed(byte key) {
    byte byteNumberOfKey = key / 8;
    byte bitNumberOfKey = key % 8;
    return bitRead( keysWhichArePressed[byteNumberOfKey], bitNumberOfKey);
}

bool isAnyKeyPressed(byte key) {
    byte index = key % 24;
    for (int i = 0; i < 3; i++) {
      byte transformedKey = index + (i * 24);
      byte byteNumberOfKey = transformedKey / 8;
      byte bitNumberOfKey = transformedKey % 8;
      bool keydown = bitRead( keysWhichArePressed[byteNumberOfKey], bitNumberOfKey);
      if (keydown) return true;
    }
    return false;
}

void loop()
{

    unsigned long currentTime = millis();
    unsigned long sixtyFourth = 0;
    if (previous > currentTime) {     
      // overflow occurred
      sixtyFourth = ((0xffffffff - previous) + currentTime) / millis_per_64th;
    } else {
      sixtyFourth = currentTime / millis_per_64th;
    }
    previous = currentTime;
    
    if (midiA.read()) {
        //Serial.printf("*** %x (%x)\n", sixtyFourth, previousSixtyFourth);
        shouldUpdatePiano = true;
        switch (midiA.getType () ) {
          case midi::NoteOn:
          case midi::NoteOff: {

              unsigned long q = 0;
              
              if (firstNote) {
                firstNote = false;
              } else {
                q = sixtyFourth - previousSixtyFourth;
              }
              previousSixtyFourth = sixtyFourth;
              midi_writer.addEvent(q, midiA.getType(), midiA.getData1(), midiA.getData2(), midiA.getChannel());
              //Serial.printf("%x: %x %x %x %x\n", q, midiA.getType(), midiA.getData1(), midiA.getData2(), midiA.getChannel());
            
            break;
          }
          
          default:
            break;
        }

        
        switch (midiA.getType () ) {
          case midi::NoteOn: {
            byte byteNumberOfKey = midiA.getData1() / 8;
            byte bitNumberOfKey = midiA.getData1() % 8;

            //Serial.printf("ON: %d,%d ON\n", byteNumberOfKey, bitNumberOfKey);
            byte b = keysWhichArePressed[byteNumberOfKey];
            //Serial.printf("before ON: %d,%d ::%x\n", byteNumberOfKey, bitNumberOfKey, b);
            bitSet( b, bitNumberOfKey);
            //Serial.printf("ON: %d,%d ::%x\n", byteNumberOfKey, bitNumberOfKey, b);
            keysWhichArePressed[byteNumberOfKey] = b;
            break;
          } 
          case midi::NoteOff: {
            byte byteNumberOfKey = midiA.getData1() / 8;
            byte bitNumberOfKey = midiA.getData1() % 8;
            byte b = keysWhichArePressed[byteNumberOfKey];
            //Serial.printf("before OFF: %d,%d ::%x\n", byteNumberOfKey, bitNumberOfKey, b);
            bitClear( b, bitNumberOfKey);
            keysWhichArePressed[byteNumberOfKey] = b;
            //Serial.printf("OFF: %d,%d ::%x\n", byteNumberOfKey, bitNumberOfKey, b);
            break;
          }   
          default:
            break;
        }
        
    } else {
      
      if (millis() - lastDisplayUpdate > 25) {
        // update display
        beat = (sixtyFourth / 64);
        bar = beat / 4;
        beat %= 4;

        if (beat != lastbeat || shouldUpdatePiano) {

          //tft.fillScreen(ST7735_BLACK);
          tft.setCursor(0,0);
          tft.setTextSize(3);
          tft.setTextColor(ST7735_BLACK);
          char c[] = "     ";
          itoa(lastbar,c,10);
          tft.print(c);
          tft.print(":");
          itoa(lastbeat+1,c,10);    
          tft.print(c); 


          drawPiano();
          shouldUpdatePiano = false;
      
    
          tft.setCursor(0,0);
  
          //tft.setTextSize(3);
          tft.setTextColor(ST7735_RED);
          
          itoa(bar,c,10);
          tft.print(c);
          tft.print(":");
          itoa(beat+1,c,10);    
          tft.print(c); 

          lastDisplayUpdate = millis(); 
          lastbeat = beat;
          lastbar = bar;
        }

  

    }
  }
}


void drawPiano() {
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
        tft.fillRect(0,i*5,32,4,ST7735_BLUE);
      else 
        tft.fillRect(0,i*5,32,4,ST7735_WHITE);     
    }
    //Draw the black keys on the keyboard
    tft.fillRect(0,3,16,3, isAnyKeyPressed(3)? ST7735_BLUE : ST7735_BLACK);
    tft.fillRect(0,8,16,3, isAnyKeyPressed(1)? ST7735_BLUE : ST7735_BLACK);
    
    tft.fillRect(0,18,16,3, isAnyKeyPressed(10)? ST7735_BLUE : ST7735_BLACK);
    tft.fillRect(0,23,16,3, isAnyKeyPressed(9)? ST7735_BLUE : ST7735_BLACK);
    tft.fillRect(0,28,16,3, isAnyKeyPressed(7)? ST7735_BLUE : ST7735_BLACK);
    
    tft.fillRect(0,38,16,3, isAnyKeyPressed(3)? ST7735_BLUE : ST7735_BLACK);
    tft.fillRect(0,43,16,3, isAnyKeyPressed(1)? ST7735_BLUE : ST7735_BLACK);
    
    tft.fillRect(0,53,16,3, isAnyKeyPressed(10)? ST7735_BLUE : ST7735_BLACK);
    tft.fillRect(0,58,16,3, isAnyKeyPressed(9)? ST7735_BLUE : ST7735_BLACK);
    tft.fillRect(0,63,16,3, isAnyKeyPressed(7)? ST7735_BLUE : ST7735_BLACK);
    
    tft.fillRect(0,73,16,3, isAnyKeyPressed(3)? ST7735_BLUE : ST7735_BLACK);
    tft.fillRect(0,78,16,3, isAnyKeyPressed(1)? ST7735_BLUE : ST7735_BLACK);
    
    tft.fillRect(0,88,16,3, isAnyKeyPressed(10)? ST7735_BLUE : ST7735_BLACK);
    tft.fillRect(0,93,16,3, isAnyKeyPressed(9)? ST7735_BLUE : ST7735_BLACK);
    tft.fillRect(0,98,16,3, isAnyKeyPressed(7)? ST7735_BLUE : ST7735_BLACK);
    
    tft.fillRect(0,108,16,3, isAnyKeyPressed(3)? ST7735_BLUE : ST7735_BLACK);
    tft.fillRect(0,113,16,3, isAnyKeyPressed(1)? ST7735_BLUE : ST7735_BLACK);

}


void drawPiano2() {
    //Draw the white keys on the keyboard
    for (int i=0; i<24; i++) {
      tft.fillRect(0,i*5,64,4,ST7735_WHITE);
    }
    //Draw the black keys on the keyboard
    tft.fillRect(0,3,32,3,ST7735_BLACK);
    tft.fillRect(0,8,32,3,ST7735_BLACK);
    
    tft.fillRect(0,18,32,3,ST7735_BLACK);
    tft.fillRect(0,23,32,3,ST7735_BLACK);
    tft.fillRect(0,28,32,3,ST7735_BLACK);
    
    tft.fillRect(0,38,32,3,ST7735_BLACK);
    tft.fillRect(0,43,32,3,ST7735_BLACK);
    
    tft.fillRect(0,53,32,3,ST7735_BLACK);
    tft.fillRect(0,58,32,3,ST7735_BLACK);
    tft.fillRect(0,63,32,3,ST7735_BLACK);
    
    tft.fillRect(0,73,32,3,ST7735_BLACK);
    tft.fillRect(0,78,32,3,ST7735_BLACK);
    
    tft.fillRect(0,88,32,3,ST7735_BLACK);
    tft.fillRect(0,93,32,3,ST7735_BLACK);
    tft.fillRect(0,98,32,3,ST7735_BLACK);
}

