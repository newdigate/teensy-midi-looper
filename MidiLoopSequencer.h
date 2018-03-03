#ifndef MidiLoopSequencer_h
#define MidiLoopSequencer_h

#include "Arduino.h"
#include <MIDI.h>
#include "midi.hpp"

#include <vector>
#include <functional>

using namespace std;

struct SongPosition {
  int bar;
  byte beat;
};


class MidiLoopSequencer
{
  public:
    MidiLoopSequencer(midi::MidiInterface<HardwareSerial> *midiPort);

    vector<function<void()>> onKeyChanged;
    vector<function<void(bool playing)>> onPlayChanged;
    vector<function<void(bool recording)>> onRecordChanged;
    vector<function<void(float tempo)>> onTempoChanged;

    void initialize();
    void tick(unsigned long milliseconds);
    
    SongPosition getSongPosition();
    void setSongPosition(int bar, byte beat);

    bool getLoopEnabled();
    void setLoopEnabled(bool loop_enabled);
    
    int getLoopDuration();
    void setLoopDuration(int bars);
    
    bool getOverdubEnabled();
    void setOverdubEnabled(bool overdub_enabled);
    
    char* getFilename();
    void setFilename(char* filename);
    
    bool getPlayEnable();
    void setPlayEnable(bool play_enabled);
    
    bool getRecordEnabled();    
    void setRecordEnabled(bool record_enabled);
    
    bool getStepRecordEnabled();
    void setStepRecordEnabled(bool step_record_enabled);
    
  private: 
    midi::MidiInterface<HardwareSerial> *_midi_port;

    char *_fileName;
    bool 
      _en_loop = false, 
      _en_rec = false, 
      _en_overdub = false,
      _en_play = false,
      _en_midiThru = true,
      _en_step_record = false,
      _en_metronome = false,
      _en_solo = false;
      
    int _loop_duration_bars = 16;
    unsigned long _milliseconds = 0;
    unsigned long _previousMilliseconds = 0;
    unsigned long _sixtyFourth = 0;
    unsigned long _previousSixtyFourth = 0;
    unsigned long _lastEventMillis = 0;
        
    SongPosition _position = { 1, 1 };
    byte _numTracks = 0;
    byte _selectedTrackIndex = 0;

    float _beats_per_minute = 0;
    float _millis_per_16th = 0;

    void allNotesOff();
    void updateBarAndBeat(unsigned long millisecs);
    void processNewIncomingMidiMessages();
    bool isNoteEvent(byte eventType);
};

#endif
