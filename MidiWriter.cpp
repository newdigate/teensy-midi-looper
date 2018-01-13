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


void MidiWriter::write_buf_int(unsigned int data) {
  //Serial.printf("%x\n", data);
  byte b = data >> 24;
  //Serial.printf("%x.", b);
  write_buf_byte( b );
  
  b = data >> 16;
  //Serial.printf("%x.", b);
  write_buf_byte( b );
   
  b = data >> 8;
  //Serial.printf("%x.", b);
  write_buf_byte( b );

  b = data;
  //Serial.printf("%x\n", b);
  write_buf_byte( b );
}

void MidiWriter::write_buf_byte(byte b) {
  if (_bufferPos > 40) {
    flush();
  }
  _buffer[_bufferPos++] = b;
}
    
void MidiWriter::setFilename(const char* filename) {
  flush();
  
  sprintf(_filename, "%s.mid", filename);
  
  int count = 1;
  while (SD.exists(_filename)) {
    //Serial.printf("'%s' already exists...\n", _filename);   
    sprintf(_filename, "%s%i.mid", filename, count);
    count++;
  }
  //Serial.printf("using filename '%s'\n", _filename);
}

void MidiWriter::writeHeader() {
  for (unsigned int i=0; i<sizeof(header); i++)
    write_buf_byte(header[i]);
    flush();
}

void MidiWriter::addEvent(int ticks, byte a, byte b, byte c, byte d) {
  write_buf_int(ticks);
  write_buf_byte(a);
  write_buf_byte(b);
  write_buf_byte(c);
  write_buf_byte(d);
  trackSize += 8;
}

void MidiWriter::flush() {
  if (_bufferPos == 0) return;
  
  File data = SD.open(_filename, FILE_WRITE);
  if (!data) {
    //Serial.printf("Not able to open %s\n", _filename);
    return;
  }
  
  for (byte b = 0; b < _bufferPos; b++) {
    data.write(_buffer[b]);
  } 
  _bufferPos = 0;
  
  data.seek(18);
  write_buf_int(trackSize);
  for (byte b = 0; b < _bufferPos; b++) {
    data.write(_buffer[b]);
  } 
  _bufferPos = 0;
  
  data.close();
}
