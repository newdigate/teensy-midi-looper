![Teensy 3.6 breadboard view](https://raw.githubusercontent.com/newdigate/arduino-midi-writer/master/Hardware/svg/teensy3.6_breadboard.svg?sanitize=true "Teensy 3.6 microcontroller board")

# Teensy midi looper 
This teensy platform micro-controller project aims to create a prototype device which can:
* **record** midi events from a physical midi input port to standard midi files (SMF) on SD-card
* **play** smf files from SD-card to a physical midi output port.  

## Work-in-progress
The project is a **work-in-progress**, and is intended for educational and enthusiast purposes. Please use with causion, and sensability. Eventually I would like to prepare the project documentation as a step-by-step tutorial that students can follow.

## Software development
 * information about **compiling** and **debugging** software here: [software](Software)

<img src="Software/docs/images/tft_emulator.png" width="200px"/>

## Prototype hardware requirements / compatibity:
  * teensy 3.6 microcontroller board 
    * onboard SD Card using SDIO
    * https://www.pjrc.com/store/teensy36.html
  * Any Adafruit_GFX compatible tft should work theorerically,
    * Im using 2.2" Adafruit_ST7735 TFT 16-bit color display 160x160
  * rotary encoder with built-in switch
  * MIDI break-out board 
    * http://www.hobbytronics.co.uk/midi-breakout
    
more information about **hardware prototyping** and **pcb design** here: [hardware](Hardware)

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
  
## Why?
I'd like a portable battery-powered device with physical midi input and output port (5-pin DIN connector) which can play and record to/from SD-card. And I'd like it to display various indicators on a cheep 2" TFT display: tempo; song position; midi channel in/out activity; keyboard view; play/stop/recording;

Eventually I'd like looping/step-recording/multi-track sequencing and recording/tempo detection, quantization. A live midi peformance tool, basically. 
