#include <MIDI.h>
#include <cppQueue.h>
#include "MidiWriter.h"
#include "TFTPianoDisplay.h"
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
TFTPianoDisplay piano(tft, 3, 0);

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
            piano.keyDown(midiA.getData1());
            break;
          } 
          case midi::NoteOff: {
            piano.keyUp(midiA.getData1());
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

        if (beat != lastbeat || piano.displayNeedsUpdating() ) {

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


          piano.drawPiano();
          
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
