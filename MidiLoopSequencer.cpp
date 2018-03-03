
#include "Arduino.h"
#include <MIDI.h>
#include "MidiLoopSequencer.h"


MidiLoopSequencer::MidiLoopSequencer(HardwareSerial *midiPort) {
  midi_port = midiPort;
}


void MidiLoopSequencer::tick() {
}
   

SongPosition MidiLoopSequencer::getSongPosition() {
  return _position;
}


void MidiLoopSequencer::setSongPosition(int bar, byte beat) {
  _position.bar = bar;
  _position.beat = beat;
}


void MidiLoopSequencer::setLoopEnabled(bool loop_enabled) {
  en_loop = loop_enabled;
}


int MidiLoopSequencer::getLoopDuration() {
  return en_loop;
}


void MidiLoopSequencer::setLoopDuration(int bars) {
  loop_duration_bars = bars;
}


bool MidiLoopSequencer::getOverdubEnabled() {
  return en_overdub;
}


void MidiLoopSequencer::setOverdubEnabled(bool overdub_enabled) {
  en_overdub = overdub_enabled;
}


char* MidiLoopSequencer::getFilename() {
  return fileName;
}


void MidiLoopSequencer::setFilename(char* filename) {
  fileName = filename;
}


bool MidiLoopSequencer::getPlayEnable() {
  return en_play;
}


void MidiLoopSequencer::setPlayEnable(bool play_enabled) {
  en_play = play_enabled;
}


bool MidiLoopSequencer::getRecordEnabled() {
  return en_rec;
}


void MidiLoopSequencer::setRecordEnabled(bool record_enabled) {
  en_rec = record_enabled;
}


bool MidiLoopSequencer::getStepRecordEnabled() {
  return en_step_record;
}


void MidiLoopSequencer::setStepRecordEnabled(bool step_record_enabled) {
  en_step_record = step_record_enabled;
}

// private

void MidiLoopSequencer::allNotesOff() {
}
