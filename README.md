# arduino-midi-writer
I would like a simple device to record the midi output of my digital piano onto an SD-card. 

Having searched the internet for a simple arduino library to write standard midi files (SMF) and not found anything, I have written a library to this. 

* classes
  * MidiWriter.h
    * class to write simple midi events to SMF on SD card 
    * currently saves single track SMF (SMF type 0)

  * TFTPianoDisplay.h
    * class to display incomming midi note on/off messages on a piano keyboard view on ILI9341 TFT (using AdafruitGFX)

  * MidiLoopSequencer.h (work in progress)
    * class to manage looping / recording / playing / event callbacks

* hardware
  * teensy 3.6 microcontroller board w/ SD Card
    * https://www.pjrc.com/store/teensy36.html
  * ILI9341 TFT display
  * rotary encoder
  * MIDI break-out board 
    * http://www.hobbytronics.co.uk/midi-breakout
  
* TFT display
  * indicators
    * displays notes received via midi on a piano keyboard view
    * song position (beats / bars)
    * indicates if SD card is missing
  * working on integrating with arduino menu
    * using rotary encoder and switch input

# work in progress
  * playback
  * looping
  * step record
  * multi-track loop recording
  * tempo detection

# dependencies
* arduino midi library 
  * https://github.com/PaulStoffregen/MIDI
* cppQueue 
  * https://github.com/SMFSW/Queue
* Arduino menu 
  * https://github.com/neu-rah/ArduinoMenu
* AdafruitGFX, Adafruit_ST7735
  * https://github.com/adafruit/Adafruit-GFX-Library 
  * https://github.com/adafruit/Adafruit-ST7735-Library
