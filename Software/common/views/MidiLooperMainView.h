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
#include <hardware_serial.h>
#include <iostream>

class MidiLooperMainView {
public:
    inline MidiLooperMainView(
            Adafruit_GFX &tft,
            midi::MidiInterface<HardwareSerial> &midiInterface,
            MidiLoopSequencer &sequencer) :
                _recordingIndicator(&tft, 8, 64),
                _topPianoDisplay(tft, 3, 0, 0, 0),
                _bottomPianoDisplay(tft, 3, 2, 0, 40),
                _songPositionIndicator(&tft, 0, 100)
    {
        _tft = &tft;
        _midiInterface = &midiInterface;
        _loopSequencer = &sequencer;
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
};

#endif //ARDUINO_ABSTRACTION_MIDILOOPERMAINVIEW_H
