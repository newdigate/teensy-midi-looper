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

Queue q(sizeof(Rec), 10, FIFO);
MidiWriter midi_writer;

void setup()
{
  tft.initR(INITR_GREENTAB); // initialize a ST7735R chip, green tab
  tft.setTextWrap(true);
  tft.fillScreen(ST7735_BLACK);
  //while (!Serial) {
  //  delay(100);
  //}
    //Serial.begin(9600);
    //Serial.println("hello!");
    
    // Initiate MIDI communications, listen to all channels
    midiA.begin(MIDI_CHANNEL_OMNI);
    //Serial.println("midi has begun!");
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


   //Serial.print("Initializing SD card...");
  
    // see if the card is present and can be initialized:
    if (!SD.begin(chipSelect)) {
      //Serial.println("Card failed, or not present");
      // don't do anything more:
      return;
    }
    //Serial.println("card initialized.");
    midi_writer.setFilename("f");
    midi_writer.writeHeader();
    midi_writer.flush();
}

unsigned int previousSixtyFourth=0;

const float beats_per_minute = 120.0;
const float beats_per_second = beats_per_minute / 60;
const float seconds_per_beat = 60 / beats_per_minute;
const float millis_per_beat = seconds_per_beat * 1000;
const float millis_per_64th = millis_per_beat/64;

byte beat;
byte bar;

void loop()
{
    unsigned int sixtyFourth = millis() / millis_per_64th;
  
    if (midiA.read()) {

        switch (midiA.getType () ) {
          case midi::NoteOn:
          case midi::NoteOff: {
      
              Rec r;
              r.time = sixtyFourth;
              r.entry1 = midiA.getType();
              r.entry2 = midiA.getData1();
              r.entry3 = midiA.getData2();
              r.entry4 = midiA.getChannel();
                    
              q.push(&r);
            
            break;
          }
          
          default:
            break;
        }
    } else
    {
      if (!q.isEmpty()) {
        
        Rec rec = {0x0000, 0xff,0xff, 0x0, 0x0};
        q.pop(&rec);
        //Serial.print();

        tft.setTextColor(ST7735_RED);
        char c[] = "  ";
        itoa(rec.entry1,c,16);
        tft.print(c);
        tft.print(" ");
        itoa(rec.entry2,c,16);
        tft.print(c);
        tft.print(" ");
        itoa(rec.entry3,c,16);
        tft.print(c);
        tft.print(" ");
        itoa(rec.entry4,c,16);
        tft.println(c);

        unsigned int q = sixtyFourth - previousSixtyFourth;
       
        midi_writer.addEvent(q, rec.entry1, rec.entry2, rec.entry3, rec.entry4);
        midi_writer.flush();
        previousSixtyFourth = sixtyFourth;        
      }

    }
}
