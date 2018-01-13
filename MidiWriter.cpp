#include "Arduino.h"
#include "MidiWriter.h"

const byte header[] = {
     0x4d, 0x54, 0x68, 0x64, 0x00, 0x00, 0x00, 0x06,
     0x00, 0x00, // single-track format
     0x00, 0x01, // one track
     0x00, 0x10, // 16 ticks per quarter
     0x4d, 0x54, 0x72, 0x6B
};


MidiWriter::MidiWriter(){
}

void MidiWriter::setFilename(const char* filename) {
  flush();
  dataFile = SD.open(filename, FILE_WRITE);
}

void MidiWriter::writeHeader() {
  for (unsigned int i=0; i<sizeof(header); i++)
    dataFile.write(header[i]);
}

void MidiWriter::addEvent(int ticks, byte a, byte b, byte c, byte d) {
  dataFile.write(ticks);
  dataFile.write(a);
  dataFile.write(b);
  dataFile.write(c);
  dataFile.write(d);
  trackSize += 4;
}

void MidiWriter::flush() {
  if (dataFile != NULL) {
    dataFile.flush();
    unsigned long position = dataFile.position();
    dataFile.seek(18);
    dataFile.write(trackSize);
    dataFile.seek(position);
  }
}
