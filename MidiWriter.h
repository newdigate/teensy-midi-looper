#ifndef MidiWriter_h
#define MidiWriter_h

#include "Arduino.h"
#include <SD.h>

class MidiWriter
{
  public:
    MidiWriter(int pin);
    void setFilename(const char* filename);
    void writeHeader();
    void addEvent(byte a, byte b, byte c, byte d);
    void flush();
  private:
    File dataFile;
    int _pin;
};

#endif
