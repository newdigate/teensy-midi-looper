//
// Created by Nicholas Newdigate on 17/03/2018.
//

#ifndef ARDUINO_ABSTRACTION_MIDILOOPERMAINVIEW_H
#define ARDUINO_ABSTRACTION_MIDILOOPERMAINVIEW_H

#if ARDUINO >= 100
#include "Arduino.h"
#include <Adafruit_GFX.h>    // Core graphics library
#include <MIDI.h>    // Core graphics library
#else
#include "mock_arduino.h"
#include "Adafruit/Adafruit_GFX.h"    // Core graphics library
#include "../../arduino/midi/MIDI.h"
#include <hardware_serial.h>
#endif

#include "../controls/TFTFlashingIndicator.h"
#include "../TFTPianoDisplay.h"
#include "../MidiLoopSequencer.h"
#include "../controls/TFTSongPositionIndicator.h"
#include "../controls/TFTSongTimeIndicator.h"
#include <cstdint>

class MidiLooperMainView {
public:
    inline MidiLooperMainView(
            Adafruit_GFX &tft,
            midi::MidiInterface<HardwareSerial> &midiInterface,
            MidiLoopSequencer &sequencer) :
                _recordingIndicator(&tft, (6*8), 0),
                _topPianoDisplay(tft, 3, 2, 0, 16),
                _bottomPianoDisplay(tft, 3, 5, 0, 30),
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
    uint64_t _lastPianoDisplayUpdate = 0;
    uint64_t _lastUpdate = 0;

};

#endif //ARDUINO_ABSTRACTION_MIDILOOPERMAINVIEW_H
