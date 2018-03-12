# compatibilty for x86 / x64 / arm 
I am writing these c++ classes with compatibility for both x86 and arduino/teensy to allow me to debug the code without needing to upload the compiled binaries a microcontroller to test; 

(I am thinking about implementing some form of mock tft display for use when debugging locally on my x86 platform, perhaps using JUCE)  

# build, run and test on x86/x64
* You need a x86/x64 compatible c++11 toolchain installed
  * build system uses `cmake` (see CMakeLists.txt in each directory)
  * JetBrains CLion IDE
    * Very good integrated development environment. integrates very well with cmake.  
      
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
