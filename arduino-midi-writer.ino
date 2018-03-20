#include <MIDI.h>
#include <cppQueue.h>
#include "Software/common/MidiWriter.h"
#include "Software/common/TFTPianoDisplay.h"
#include "Software/common/MidiLoopSequencer.h" 

#include "Software/common/MidiWriter.cpp"
#include "Software/common/TFTPianoDisplay.cpp"
#include "Software/common/MidiLoopSequencer.cpp" 

#define sclk 13  // SCLK can also use pin 14
#define mosi 11  // MOSI can also use pin 7
#define cs   10  // CS & DC can use pins 2, 6, 9, 10, 15, 20, 21, 22, 23
#define dc   9   //  but certain pairs must NOT be used: 2+10, 6+9, 20+23, 21+22
#define rst  8   // RST can use any pin
#define sdcs 4   // CS for SD card, can use any pin

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

// workaround for linking error when including c++ std library for teensy 
// https://forum.arduino.cc/index.php?topic=382211.0
#if defined(TEENSY)
namespace std { void __throw_bad_function_call() { Serial.print("throw_bad_function_call"); } }
#endif

// Teensy 3.5 & 3.6 on-board: BUILTIN_SDCARD
// Wiz820+SD board: pin 4
// Teensy 2.0: pin 0
// Teensy++ 2.0: pin 20
#ifndef BUILTIN_SDCARD
#define BUILTIN_SDCARD 10
#endif
const int chipSelect = BUILTIN_SDCARD;
const byte numOctaves = 3;
const byte startOctave = 2;
Adafruit_ST7735 tft = Adafruit_ST7735(cs, dc, rst);
TFTPianoDisplay piano(tft, numOctaves, startOctave, 5, 24);
TFTPianoDisplay piano2(tft, numOctaves, startOctave+numOctaves, 5, 80);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1,     midiA);
MidiWriter midi_writer;

bool _sdCardFound = false;

MidiLoopSequencer sequencer( &midiA );

void setup()
{
  tft.initR(INITR_GREENTAB); // initialize a ST7735R chip, green tab
  tft.setRotation(2);
  tft.setTextWrap(true);
  tft.fillScreen(ST7735_BLACK);
  //while (!Serial) {
  //  delay(100);
  //}
    Serial.begin(9600);
    Serial.println("hello!");

    sequencer.initialize();
    // Initiate MIDI communications, listen to all channels
    // midiA.begin(MIDI_CHANNEL_OMNI);
    //Serial.println("midi has begun!");



   //Serial.print("Initializing SD card...");
  
    // see if the card is present and can be initialized:
    if (!SD.begin(chipSelect)) {
        _sdCardFound = false;
        tft.setCursor(16,64);
        tft.setTextSize(1);
        tft.setTextColor(ST7735_RED);   
        tft.print("SD card not found");
        return;
    } else 
      _sdCardFound = true;
      
    //Serial.println("card initialized.");
    midi_writer.setFilename("rec");
    midi_writer.writeHeader();
    midi_writer.flush();

  sequencer.onKeyChanged += [&] (bool isPressed, byte key, byte velocity, byte channle) {
      Serial.print("key released:");
      Serial.print(key);
      Serial.println();
    
  };

}

unsigned long previousSixtyFourth=0;
const float beats_per_minute = 120.0;
const float beats_per_second = beats_per_minute / 60;
const float seconds_per_beat = 60 / beats_per_minute;
const float millis_per_beat = seconds_per_beat * 1000;
const float millis_per_16th = millis_per_beat/16;
const float millis_per_64th = millis_per_beat/64;
const float millis_per_256th = millis_per_beat/256;
int beat, lastbeat, bar, lastbar;

bool firstNote = true;
unsigned long previous = 0;
unsigned long lastDisplayUpdate = 0;
unsigned long lastDisplayFilenameUpdate = 0;
unsigned long lastsdCardCheck = 0;
unsigned long lastDisplayRecordIndictorBlink = 0;
unsigned long lastEvent = 0;

bool recordIndicatorState = false;
bool sdcardIndicatorState = false;

const bool enableMidiThru = true;

void loop() {
  
    unsigned long currentTime = millis();

    sequencer.tick(currentTime);
    
    unsigned long sixtyFourth = 0;
    if (previous > currentTime) {     
      // overflow occurred
      sixtyFourth = ((0xffffffff - previous) + currentTime) / millis_per_16th;
    } else {
      sixtyFourth = currentTime / millis_per_16th;
    }
    previous = currentTime;
    
  
        
    if (piano.displayNeedsUpdating()) {
      piano.drawPiano();
    } 
    
    if (piano2.displayNeedsUpdating()) {
      piano2.drawPiano();
    } else
    
    if (_sdCardFound && (lastDisplayFilenameUpdate == 0 || currentTime - lastDisplayFilenameUpdate > 10000)) {
        tft.setCursor(16,64);
        tft.setTextSize(1);
        tft.fillRect(0, 64, 128, 16, ST7735_BLACK);
        tft.setTextColor(ST7735_RED);   
        tft.print(midi_writer.getFilename());
        lastDisplayFilenameUpdate = currentTime;
    }

    if ( _sdCardFound && (lastDisplayRecordIndictorBlink == 0 || currentTime - lastDisplayRecordIndictorBlink > 500)) {
      lastDisplayRecordIndictorBlink = currentTime;
      recordIndicatorState = !recordIndicatorState;
      tft.fillCircle(8,64+4, 4,recordIndicatorState? ST7735_RED : ST7735_BLACK);
    }

    if ( !_sdCardFound && (lastsdCardCheck == 0 || currentTime - lastsdCardCheck > 500) )  {
      if (!SD.begin(chipSelect)) {
        _sdCardFound = false;
        tft.fillRect(0, 64, 128, 16, ST7735_BLACK);
        tft.setTextSize(1); 
        tft.setCursor(2,64);
        sdcardIndicatorState = !sdcardIndicatorState;
        tft.setTextColor(sdcardIndicatorState? ST7735_YELLOW : ST7735_BLACK); 
        tft.print("insert SD card");
        lastsdCardCheck = millis();
        return;
      } else {
        _sdCardFound = true;
  
        tft.fillRect(0, 64, 128, 16, ST7735_BLACK);
        tft.setTextSize(1); 
        tft.setCursor(2,64);
        tft.setTextColor(ST7735_YELLOW); 
        tft.print("found SD card");
        delay(1000);
        
        midi_writer.setFilename("rec");
        midi_writer.writeHeader();
        midi_writer.flush();
  
        tft.fillRect(0, 64, 128, 16, ST7735_BLACK);
        tft.setTextSize(1); 
        tft.setCursor(2,64);
        tft.setTextColor(ST7735_YELLOW); 
        tft.print("initialized....");
        
        lastsdCardCheck = millis();
      }
    }
    if (currentTime - lastDisplayUpdate > 50) {
      // update display
      beat = (sixtyFourth / 16);
      bar = beat / 4;
      beat %= 4;
      
      
      
      if (beat != lastbeat ) {

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



        tft.setCursor(0,0);

        //tft.setTextSize(3);
        tft.setTextColor(ST7735_GREEN);
        
        itoa(bar,c,10);
        tft.print(c);
        tft.print(":");
        itoa(beat+1,c,10);    
        tft.print(c); 

        lastDisplayUpdate = currentTime; 
        lastbeat = beat;
        lastbar = bar;
      }

    if (currentTime - lastEvent > 30000) {
        midi_writer.setFilename("rec");
        midi_writer.writeHeader();
        midi_writer.flush(); 
        firstNote = true;    
    }

  }

}
