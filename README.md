# arduino-midi-writer
* arduino class to write simple midi events to SMF on SD card 
  * currently saves single track SMF
  
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
