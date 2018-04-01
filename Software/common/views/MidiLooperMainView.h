//
// Created by Nicholas Newdigate on 17/03/2018.
//

#ifndef ARDUINO_ABSTRACTION_MIDILOOPERMAINVIEW_H
#define ARDUINO_ABSTRACTION_MIDILOOPERMAINVIEW_H

#include "../../arduino/midi/MIDI.h"
#include "../controls/TFTFlashingIndicator.h"
#include "../TFTPianoDisplay.h"
#include "../MidiLoopSequencer.h"
#include "../controls/TFTSongPositionIndicator.h"
#include "../controls/TFTSongTimeIndicator.h"
#include <hardware_serial.h>
#include <iostream>

class MidiLooperMainView {
public:
    inline MidiLooperMainView(
            Adafruit_GFX &tft,
            midi::MidiInterface<HardwareSerial> &midiInterface,
            MidiLoopSequencer &sequencer) :
                _recordingIndicator(&tft, 8, 100),
                _topPianoDisplay(tft, 3, 0, 0, 16),
                _bottomPianoDisplay(tft, 3, 2, 0, 56),
                _songPositionIndicator(&tft, 0, 0),
                _songTimeIndicator(&tft, 0, 8)
    {
        _tft = &tft;
        _midiInterface = &midiInterface;
        _loopSequencer = &sequencer;
        _loopSequencer->onKeyChanged += [&] (bool keyOn, byte key, byte velocity, byte channel) {
            if (keyOn) {
                _bottomPianoDisplay.keyDown(key);
                _topPianoDisplay.keyDown(key);
            }
            else {
                _bottomPianoDisplay.keyUp(key);
                _topPianoDisplay.keyUp(key);
            }
        };
    }

    void update(unsigned long millis);

private:
    Adafruit_GFX *_tft;
    midi::MidiInterface<HardwareSerial> *_midiInterface;
    MidiLoopSequencer *_loopSequencer;

    TFTFlashingIndicator _recordingIndicator;
    TFTSongPositionIndicator _songPositionIndicator;
    TFTPianoDisplay _topPianoDisplay;
    TFTPianoDisplay _bottomPianoDisplay;
    TFTSongTimeIndicator _songTimeIndicator;
};

#endif //ARDUINO_ABSTRACTION_MIDILOOPERMAINVIEW_H
