# build, run and test on x86/x64
* You need a x86/x64 compatible c++11 toolchain installed
  * cmake for mac, and linux users 
    * for macos use brew to install cmake: https://brew.sh/ 
      * `brew install cmake`
  * JetBrains Clion IDE
      
* to build for x86/x64
  * open terminal to arduino_midi_writer/Software
    * build arduino abstractions library (libarduino_abstraction.a)
    ```
    $cd arduino
    $mkdir cmake-build-debug
    $cd cmake-build-debug/
    cmake ..
    make
    ```
    * build common library (libteensy_midi_common.a)
    ```
    $cd ../../common/
    $mkdir cmake-build-debug
    $cd cmake-build-debug/
    cmake ..
    make
    ```  
    * build x86/x64 application (libarduino_abstraction.a)
    ```
    $cd ../../x86/
    $mkdir cmake-build-debug
    $cd cmake-build-debug/
    cmake ..
    make
    ```  
* to run
  * follow build instructions above
open terminal to arduino_midi_writer/Software/x86/cmake-build-debug/
  ```
  ./arduino_midi_writer
  ```
# Classes
  * MidiWriter.h
    * write simple midi events to SMF on SD card 
    * currently saves single track SMF (SMF type 0)
  * TFTPianoDisplay.h
    * display incomming midi note on/off messages on a piano keyboard view
  * MidiLoopSequencer.h (work in progress)
    * manage looping / recording / playing / event callbacks
  
# Dependencies
* arduino midi library 
  * https://github.com/PaulStoffregen/MIDI
* cppQueue 
  * https://github.com/SMFSW/Queue
* Arduino menu 
  * https://github.com/neu-rah/ArduinoMenu
* AdafruitGFX, Adafruit_ST7735
  * https://github.com/adafruit/Adafruit-GFX-Library 
  * https://github.com/adafruit/Adafruit-ST7735-Library
