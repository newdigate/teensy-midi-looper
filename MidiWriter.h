#ifndef MidiWriter_h
#define MidiWriter_h

#include "Arduino.h"
#include <SD.h>

class MidiWriter
{
  public:
    MidiWriter();
    void setFilename(const char* filename);
    void writeHeader();
    void addEvent(int ticks, byte a, byte b, byte c, byte d);
    void flush();
  private:
    unsigned long trackSize = 0;
    File dataFile;
};

#endif
