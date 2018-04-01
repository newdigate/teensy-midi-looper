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
#include <SD/SD.h>
//==============================================================================
/*
    This component lives inside our window, and this is where you sh	ould put all
    your controls and content.
*/

uint8_t tab[][3] = {
    {0x90, 0x76, 0x64},
    {0x80, 0x76, 0x64}
};

class MainContentComponent   : public AnimatedAppComponent, public MidiInputCallback,  public MidiKeyboardStateListener
{
public:
    //==============================================================================
    MainContentComponent()   :
        keyboardComponent (keyboardState, MidiKeyboardComponent::horizontalKeyboard),
        startTime (Time::getMillisecondCounterHiRes() * 0.001)
    {
        setOpaque (true);
        setSize (_tft_emulatorWidth, _tft_emulatorHeight+_headerHeight);
        setFramesPerSecond (100);
        initialize_mock_arduino();
        SDClass::setSDCardFolderPath("/Users/nicnewdigate/Development/sdcard");
        sequencer.initialize();
        sequencer.setPlayEnable(true);
        sequencer.onKeyChanged += [] (bool keyOn, byte key, byte velocity, byte channel) {
            // cout << keyOn << " " << key << " " << velocity << " " << channel << "\n";
        };
        sequencer.onPositionChanged += [] (SongPosition p) {
            printf( "P: %d : %d \n", p.bar, p.beat);
        };
        
        addAndMakeVisible (midiInputListLabel);
        midiInputListLabel.setText ("MIDI Input:", dontSendNotification);
        midiInputListLabel.attachToComponent (&midiInputList, true);
        
        addAndMakeVisible (midiInputList);
        
        keyboardState.addListener (this);
        
        midiInputList.setTextWhenNoChoicesAvailable ("No MIDI Inputs Enabled");
        auto midiInputs = MidiInput::getDevices();
        midiInputList.addItemList (midiInputs, 1);
        midiInputList.onChange = [this] { setMidiInput (midiInputList.getSelectedItemIndex()); };
        // find the first enabled device and use that by default
        for (auto midiInput : midiInputs)
        {
            if (deviceManager.isMidiInputEnabled (midiInput))
            {
                setMidiInput (midiInputs.indexOf (midiInput));
                break;
            }
        }
        // if no enabled devices were found just use the first one in the list
        if (midiInputList.getSelectedId() == 0)
            setMidiInput (0);
        //image.setPixelAt(0, 0, juce::Colour(100,100,250));
        
        addAndMakeVisible (midiMessagesBox);
        midiMessagesBox.setMultiLine (true);
        midiMessagesBox.setReturnKeyStartsNewLine (true);
        midiMessagesBox.setReadOnly (true);
        midiMessagesBox.setScrollbarsShown (true);
        midiMessagesBox.setCaretVisible (false);
        midiMessagesBox.setPopupMenuEnabled (true);
        midiMessagesBox.setColour (TextEditor::backgroundColourId, Colour (0x32ffffff));
        midiMessagesBox.setColour (TextEditor::outlineColourId, Colour (0x1c000000));
        midiMessagesBox.setColour (TextEditor::shadowColourId, Colour (0x16000000));
        
        addAndMakeVisible (keyboardComponent);
        keyboardState.addListener (this);
    }
    
    void handleIncomingMidiMessage (MidiInput* source, const MidiMessage& message) override
    {
        const ScopedValueSetter<bool> scopedInputFlag (isAddingFromMidiInput, true);
        keyboardState.processNextMidiEvent (message);
        postMessageToList (message, source->getName());
    }
    
    void setMidiInput (int index)
    {
        auto list = MidiInput::getDevices();
        deviceManager.removeMidiInputCallback (list[lastInputIndex], this);
        auto newInput = list[index];
        if (! deviceManager.isMidiInputEnabled (newInput))
            deviceManager.setMidiInputEnabled (newInput, true);
        deviceManager.addMidiInputCallback (newInput, this);
        midiInputList.setSelectedId (index + 1, dontSendNotification);
        lastInputIndex = index;
    }
    
    void handleNoteOn (MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override
    {
        if (! isAddingFromMidiInput)
        {
            auto m = MidiMessage::noteOn (midiChannel, midiNoteNumber, velocity);
            m.setTimeStamp (Time::getMillisecondCounterHiRes() * 0.001);
            postMessageToList (m, "On-Screen Keyboard");
        }
    }
    
    void handleNoteOff (MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override
    {
        if (! isAddingFromMidiInput)
        {
            auto m = MidiMessage::noteOff (midiChannel, midiNoteNumber);
            m.setTimeStamp (Time::getMillisecondCounterHiRes() * 0.001);
            postMessageToList (m, "On-Screen Keyboard");
        }
    }
    
    // This is used to dispach an incoming message to the message thread
    class IncomingMessageCallback   : public CallbackMessage
    {
    public:
        IncomingMessageCallback (MainContentComponent* o, const MidiMessage& m, const String& s)
        : owner (o), message (m), source (s)
        {}
        
        void messageCallback() override
        {
            if (owner != nullptr)
                owner->addMessageToList (message, source);
        }
        
        Component::SafePointer<MainContentComponent> owner;
        MidiMessage message;
        String source;
    };
    
    void postMessageToList (const MidiMessage& message, const String& source)
    {
        (new IncomingMessageCallback (this, message, source))->post();
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
        /*
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
        } */
        
    }

//    void paint (Graphics& g) override
//    {
//        //g.fillRect(0, 0, getWidth(), getHeight());

//
//    }
//
    void paint (Graphics& g) override
    {
        g.fillAll (Colours::black);
        g.drawImage ( image,
                    0, _headerHeight, _tft_emulatorWidth, _headerHeight + _tft_emulatorHeight,
                    0, 0, 160, 160, false);
    }
    
    void resized() override
    {
        auto area = getLocalBounds();
        
        midiInputList    .setBounds (area.removeFromTop (36).removeFromRight (getWidth() - 150).reduced (8));
        keyboardComponent.setBounds (area.removeFromTop (80).reduced(8));
        midiMessagesBox  .setBounds (area.reduced (8));
    }

    void addMessageToList (const MidiMessage& message, const String& source)
    {
        auto time = message.getTimeStamp() - startTime;
        const uint8_t *m = message.getRawData();
        Serial._inputBuffer.push(m++);
        Serial._inputBuffer.push(m++);
        Serial._inputBuffer.push(m++);
        
        auto hours   = ((int) (time / 3600.0)) % 24;
        auto minutes = ((int) (time / 60.0)) % 60;
        auto seconds = ((int) time) % 60;
        auto millis  = ((int) (time * 1000.0)) % 1000;
        
        auto timecode = String::formatted ("%02d:%02d:%02d.%03d",
                                           hours,
                                           minutes,
                                           seconds,
                                           millis);
        
        auto description = getMidiMessageDescription (message);
        
        String midiMessageString (timecode + "  -  " + description + " (" + source + ")"); // [7]
        logMessage (midiMessageString);
    }

private:
    
    static String getMidiMessageDescription (const MidiMessage& m)
    {
        if (m.isNoteOn())           return "Note on "          + MidiMessage::getMidiNoteName (m.getNoteNumber(), true, true, 3);
        if (m.isNoteOff())          return "Note off "         + MidiMessage::getMidiNoteName (m.getNoteNumber(), true, true, 3);
        if (m.isProgramChange())    return "Program change "   + String (m.getProgramChangeNumber());
        if (m.isPitchWheel())       return "Pitch wheel "      + String (m.getPitchWheelValue());
        if (m.isAftertouch())       return "After touch "      + MidiMessage::getMidiNoteName (m.getNoteNumber(), true, true, 3) +  ": " + String (m.getAfterTouchValue());
        if (m.isChannelPressure())  return "Channel pressure " + String (m.getChannelPressureValue());
        if (m.isAllNotesOff())      return "All notes off";
        if (m.isAllSoundOff())      return "All sound off";
        if (m.isMetaEvent())        return "Meta event";
        
        if (m.isController())
        {
            String name (MidiMessage::getControllerName (m.getControllerNumber()));
            
            if (name.isEmpty())
                name = "[" + String (m.getControllerNumber()) + "]";
            
            return "Controller " + name + ": " + String (m.getControllerValue());
        }
        
        return String::toHexString (m.getRawData(), m.getRawDataSize());
    }
    
    void logMessage (const String& m)
    {
        midiMessagesBox.moveCaretToEnd();
        midiMessagesBox.insertTextAtCaret (m + newLine);
    }
    
    //==============================================================================
    unsigned long _t = 0;
    const int _tft_width = 160;
    const int _tft_height = 160;
    const int _headerHeight = 200;
    const int _tft_emulatorWidth = 160 * 8;
    const int _tft_emulatorHeight = 160 * 8;
    // Your private member variables go here...
    juce::Image image = Image(juce::Image::PixelFormat::RGB, _tft_width, _tft_height, true);
    Byte octaves = 2, startOctave = 2, x = 10, y = 10;
    Adafruit_GFX_juce juceTFT = Adafruit_GFX_juce(160, 160, image);
    midi::MidiInterface<HardwareSerial> midi = midi::MidiInterface<HardwareSerial>(Serial);
    MidiLoopSequencer sequencer = MidiLoopSequencer(&midi);
    MidiLooperMainView mainView = MidiLooperMainView(juceTFT, midi, sequencer);
    //TFTPianoDisplay ff = TFTPianoDisplay(juceTFT, octaves, startOctave, x, y);
    
    int lastInputIndex = 0;                     // [3]
    bool isAddingFromMidiInput = false;         // [4]
    Label midiInputListLabel;
    MidiKeyboardState keyboardState;            // [5]
    MidiKeyboardComponent keyboardComponent;    // [6]
    TextEditor midiMessagesBox;
    double startTime;
    AudioDeviceManager deviceManager;
    ComboBox midiInputList;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()    { return new MainContentComponent(); }

