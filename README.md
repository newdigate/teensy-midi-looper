# Teensy midi loop recorder
This micro-controller project aims to play and record midi standard midi files (SMF) from SD-card.   

## Compatibilty:
I am writing these c++ classes with compatibility for both x86 and arm32 to allow me to debug the code without needing to upload the compiled binaries to the microcontroller; 

## Hardware requirements:
  * teensy 3.6 microcontroller board with onboard SD Card
    * https://www.pjrc.com/store/teensy36.html
  * 2.2" ILI9341 TFT display 240x320
  * rotary encoder with built-in switch
  * MIDI break-out board 
    * http://www.hobbytronics.co.uk/midi-breakout

* classes
  * MidiWriter.h
    * class to write simple midi events to SMF on SD card 
    * currently saves single track SMF (SMF type 0)
  * TFTPianoDisplay.h
    * class to display incomming midi note on/off messages on a piano keyboard view on ILI9341 TFT (using AdafruitGFX)
  * MidiLoopSequencer.h (work in progress)
    * class to manage looping / recording / playing / event callbacks
  
* TFT display
  * indicators
    * displays notes received via midi on a piano keyboard view
    * song position (beats / bars)
    * indicates if SD card is missing
  * working on integrating with arduino menu
    * using rotary encoder and switch input

## work in progress
  * playback
  * looping
  * step record
  * multi-track loop recording
  * tempo detection

## dependencies
* arduino midi library 
  * https://github.com/PaulStoffregen/MIDI
* cppQueue 
  * https://github.com/SMFSW/Queue
* Arduino menu 
  * https://github.com/neu-rah/ArduinoMenu
* AdafruitGFX, Adafruit_ST7735
  * https://github.com/adafruit/Adafruit-GFX-Library 
  * https://github.com/adafruit/Adafruit-ST7735-Library
