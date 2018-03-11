# Teensy midi looper 
![Teensy 3.6 breadboard view](https://raw.githubusercontent.com/newdigate/arduino-midi-writer/master/Hardware/svg/teensy3.6_breadboard.svg?sanitize=true "Teensy 3.6 microcontroller board")

This arduino/teensy platform micro-controller project aims to record midi events from a midi input port (physical) to standard midi files (SMF) on SD-card, and play smf files from SD-card to a midi output port.  

This project is a **work-in-progress**, and is intended for educational and enthusiast purposes. Please use with causion, and sensability. Eventually I would like to prepare the project documentation as a step-by-step tutorial that students can follow.

## Why?
I'd like a portable battery-powered device with physical midi input and output port (5-pin DIN connector) which can play and record to/from SD-card. And I'd like it to display various indicators-tempo, song position, midi channel in/out activity, keyboard view, play/stop/recording, on a cheep 2" TFT display. Eventually I would like looping/step-recording/multi-track sequencing and recording/tempo detection, quantization. A live midi peformance tool, basically. 

## compatibilty for x86 / x64 / arm 
I am writing these c++ classes with compatibility for both x86 and arduino/teensy to allow me to debug the code without needing to upload the compiled binaries a microcontroller to test; 

(I am thinking about implementing some form of mock tft display for use when debugging locally on my x86 platform, perhaps using JUCE)  

## Hardware requirements / compatibity:
  * teensy 3.6 microcontroller board with onboard SD Card
    * https://www.pjrc.com/store/teensy36.html
  * 2.2" Adafruit_ST7735 TFT 16-bit color display 160x160
  * rotary encoder with built-in switch
  * MIDI break-out board 
    * http://www.hobbytronics.co.uk/midi-breakout
  
## TFT display
  * indicators
    * displays notes received via midi on a piano keyboard view
    * song position (beats / bars)
    * indicates if SD card is missing
  * working on integrating with arduino menu
    * using rotary encoder and switch input

## Work in progress
  * recording (mostly working, tested lightly)
  * playback (not started)
  * looping
  * step record
  * multi-track loop recording
  * tempo detection
  * ILI9341 320x240 TFT color display integration
  * menu system
