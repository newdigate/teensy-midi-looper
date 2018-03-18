/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2017 - ROLI Ltd.

   JUCE is an open source library subject to commercial or open-source
   licensing.

   By using JUCE, you agree to the terms of both the JUCE 5 End-User License
   Agreement and JUCE 5 Privacy Policy (both updated and effective as of the
   27th April 2017).

   End User License Agreement: www.juce.com/juce-5-licence
   Privacy Policy: www.juce.com/juce-5-privacy-policy

   Or: You may also use this code under the terms of the GPL v3 (see
   www.gnu.org/licenses).

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include <TFTPianoDisplay.h>
#include <Adafruit/Adafruit_GFX.h>
#include <hardware_serial.h>
#include "../../../arduino/midi/MIDI.h"
#include "Adafruit_GFX_juce.hpp"
#include "../../../common/views/MidiLooperMainView.h"
#include "../../../common/MidiLoopSequencer.h"
#include "../../../arduino/mock_arduino.h"
//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/

uint8_t tab[][3] = {
    {0x90, 0x76, 0x64},
    {0x80, 0x76, 0x64}
};

class MainContentComponent   : public AnimatedAppComponent
{
public:
    //==============================================================================
    MainContentComponent()
    {
        setSize (_width, _height);
        setFramesPerSecond (50);
        initialize_mock_arduino();
        sequencer.initialize();
        sequencer.setPlayEnable(true);
        sequencer.onKeyChanged += [] (bool keyOn, byte key, byte velocity, byte channel) {
            // cout << keyOn << " " << key << " " << velocity << " " << channel << "\n";
        };
        sequencer.onPositionChanged += [] (SongPosition p) {
            printf( "P: %d : %d \n", p.bar, p.beat);
        };
        
        //image.setPixelAt(0, 0, juce::Colour(100,100,250));
    }

    void update() override
    {
        // ff.drawPiano();
        unsigned long m = millis();
        //cout << "milis: " << m << "\n";
        // This function is called at the frequency specified by the setFramesPerSecond() call
        // in the constructor. You can use it to update counters, animate values, etc.
        mainView.update(m);
        _t++;
        
        if (_t % 50 == 10) {
            //cout << "<- note on";
            Serial._inputBuffer.push(tab[0] + 0);
            Serial._inputBuffer.push(tab[0] + 1);
            Serial._inputBuffer.push(tab[0] + 2);
        } else if (_t % 50 == 25) {
            //cout << "<- note off";
            Serial._inputBuffer.push(tab[1] + 0);
            Serial._inputBuffer.push(tab[1] + 1);
            Serial._inputBuffer.push(tab[1] + 2);
        }
        
    }

    void paint (Graphics& g) override
    {
        g.fillRect(0, 0, getWidth(), getHeight());
        g.drawImage ( image,
                    0, 0, getWidth(), getHeight(),
                      0, 0, 160, 160, false);
    }

    void resized() override
    {
        // This is called when the MainContentComponent is resized.
        // If you add any child components, this is where you should
        // update their positions.
    }


private:
    //==============================================================================
    unsigned long _t = 0;
    const int _width = 160;
    const int _height = 160;
    // Your private member variables go here...
    juce::Image image = Image(juce::Image::PixelFormat::RGB, _width, _height, true);
    Byte octaves = 2, startOctave = 2, x = 10, y = 10;
    Adafruit_GFX_juce juceTFT = Adafruit_GFX_juce(160, 160, image);
    midi::MidiInterface<HardwareSerial> midi = midi::MidiInterface<HardwareSerial>(Serial);
    MidiLoopSequencer sequencer = MidiLoopSequencer(&midi);
    MidiLooperMainView mainView = MidiLooperMainView(juceTFT, midi, sequencer);
    TFTPianoDisplay ff = TFTPianoDisplay(juceTFT, octaves, startOctave, x, y);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()    { return new MainContentComponent(); }
