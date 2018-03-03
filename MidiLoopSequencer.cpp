#include <vector>
#include <functional>


#include "Arduino.h"
#include <MIDI.h>
#include "MidiLoopSequencer.h"


MidiLoopSequencer::MidiLoopSequencer(midi::MidiInterface<HardwareSerial> *midiPort) {
  _midi_port = midiPort;
}

void MidiLoopSequencer::initialize() {
  _midi_port->begin(MIDI_CHANNEL_OMNI);
  _beats_per_minute = 120.0;
  float seconds_per_beat = 60 / _beats_per_minute;
  float millis_per_beat = seconds_per_beat * 1000;
  _millis_per_16th = millis_per_beat/16;
  
  _milliseconds = 0;
  _previousMilliseconds = 0;
}

void MidiLoopSequencer::tick(unsigned long millisecs) {
    updateBarAndBeat(millisecs);
}
   
unsigned long MidiLoopSequencer::updateBarAndBeat(unsigned long millisecs) {
  _previousMilliseconds = _milliseconds;
  _milliseconds = millisecs;
  
  unsigned long delta;
  if (_previousMilliseconds == 0) {
    delta = _milliseconds;
    _sixtyFourth = delta / _millis_per_16th;
  } else 
  if (_previousMilliseconds > _milliseconds) {     
    // overflow occurred
    unsigned long underflow = (0xffffffff - _previousMilliseconds);
    delta = (underflow + _milliseconds);
    _sixtyFourth = delta / _millis_per_16th;
  } else {
    delta = _milliseconds - _previousMilliseconds;
    _sixtyFourth = delta / _millis_per_16th;
  }

  unsigned long beat = (_sixtyFourth / 16);
  _position.bar = beat / 4;
  _position.beat = beat % 4;

  return delta;
}

SongPosition MidiLoopSequencer::getSongPosition() {
  return _position;
}


void MidiLoopSequencer::setSongPosition(int bar, byte beat) {
  _position.bar = bar;
  _position.beat = beat;
}


void MidiLoopSequencer::setLoopEnabled(bool loop_enabled) {
  _en_loop = loop_enabled;
}


int MidiLoopSequencer::getLoopDuration() {
  return _en_loop;
}


void MidiLoopSequencer::setLoopDuration(int bars) {
  _loop_duration_bars = bars;
}


bool MidiLoopSequencer::getOverdubEnabled() {
  return _en_overdub;
}


void MidiLoopSequencer::setOverdubEnabled(bool overdub_enabled) {
  _en_overdub = overdub_enabled;
}


char* MidiLoopSequencer::getFilename() {
  return _fileName;
}


void MidiLoopSequencer::setFilename(char* filename) {
  _fileName = filename;
}


bool MidiLoopSequencer::getPlayEnable() {
  return _en_play;
}


void MidiLoopSequencer::setPlayEnable(bool play_enabled) {
  _en_play = play_enabled;
}


bool MidiLoopSequencer::getRecordEnabled() {
  return _en_rec;
}


void MidiLoopSequencer::setRecordEnabled(bool record_enabled) {
  _en_rec = record_enabled;
}


bool MidiLoopSequencer::getStepRecordEnabled() {
  return _en_step_record;
}


void MidiLoopSequencer::setStepRecordEnabled(bool step_record_enabled) {
  _en_step_record = step_record_enabled;
}

// private

void MidiLoopSequencer::allNotesOff() {
}
