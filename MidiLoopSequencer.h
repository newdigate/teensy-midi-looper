#ifndef MidiLoopSequencer_h
#define MidiLoopSequencer_h

#include "Arduino.h"
#include <MIDI.h>
#include "midi.hpp"
struct SongPosition {
  int bar;
  byte beat;
};

class MidiLoopSequencer
{
  public:
    MidiLoopSequencer(HardwareSerial *midiPort);

    void tick();
    
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
    HardwareSerial *midi_port;

    char *fileName;
    bool 
      en_loop = false, 
      en_rec = false, 
      en_overdub = false,
      en_play = false,
      en_midiThru = true,
      en_step_record = false,
      en_metronome = false,
      en_solo = false;
      
    int loop_duration_bars = 16;
    SongPosition _position = { 1, 1 };
    byte _numTracks = 0;
    byte _selectedTrackIndex = 0;

    //File *smf_tracks;

    // 

    void allNotesOff();
};

#endif
