/*

 SD - a slightly more friendly wrapper for sdfatlib

 This library aims to expose a subset of SD card functionality
 in the form of a higher level "wrapper" object.

 License: GNU General Public License V3
          (Because sdfatlib is licensed with this.)

 (C) Copyright 2010 SparkFun Electronics

 */

#include "SD.h"
#include "utility/SdFat.h"

/* for debugging file open/close leaks
   uint8_t nfilecount=0;
*/
#include <stdio.h>
#include <iostream>
#include <fstream>
using namespace std;

std::streampos fileSize( const char* filePath ){
    
    std::streampos fsize = 0;
    std::ifstream file( filePath, std::ios::binary );
    
    fsize = file.tellg();
    file.seekg( 0, std::ios::end );
    fsize = file.tellg() - fsize;
    file.close();
    
    return fsize;
}



//File::File(SdFile f, const char *n) {
File::File(SdFile f, const char *n) {
    std::string actualFileName = SDClass::_mockSDCardLocation + std::string(n);
    cout << actualFileName;
    mockFile.open(actualFileName);
    _size = fileSize(actualFileName.c_str());
}

File::File(const char *n) {
  //_file = (SdFile *)malloc(sizeof(SdFile));
    
    std::string actualFileName = SDClass::_mockSDCardLocation + std::string(n);
    cout << actualFileName;
    mockFile.open(actualFileName);
    _size = fileSize(actualFileName.c_str());
    
//  if (_file) {
//    memcpy(_file, &f, sizeof(SdFile));
//
//    strncpy(_name, n, 12);
//    _name[12] = 0;
//
//    /* for debugging file open/close leaks
//       nfilecount++;
//       Serial.print("Created \"");
//       Serial.print(n);
//       Serial.print("\": ");
//       Serial.println(nfilecount, DEC);
//    */
//  }
}

File::File(void) {
  _file = 0;
  _name[0] = 0;
    _size = 0;
  //Serial.print("Created empty file object");
}


// returns a pointer to the file name
char *File::name(void) {
  return _name;
}

// a directory is a special type of file
bool File::isDirectory(void) {
  return (_file && _file->isDir());
}


int File::write(uint8_t val) {
  return write(&val, 1);
}

int File::write(const uint8_t *buf, size_t size) {
  size_t t;
  if (!_file) {
    //setWriteError();
    return 0;
  }
    
    _size += size;
    //char * memblock = new char[size];
    char * memblock = (char *)buf;
   mockFile.write(memblock, size);
  _file->clearWriteError();
  t = _file->write(buf, size);
  if (_file->getWriteError()) {
    //setWriteError();
    return 0;
  }
  return t;
}

int File::peek() {
  if (! _file) 
    return 0;

  int c = _file->read();
  if (c != -1) _file->seekCur(-1);
  return c;
}

int File::read() {
    char p[2];
    mockFile.read(p, 2);
    return p[0] << 8 | p[1];
}

// buffered read for more efficient, high speed reading
int File::read(void *buf, uint16_t nbyte) {
    char *bbb = new char[nbyte];
    mockFile.read(bbb, nbyte);
    return nbyte;
}

int File::available() {
    if (! mockFile.is_open()) return 0;
    uint32_t p = position();
    uint32_t n = size() - p;

    return n > 0X7FFF ? 0X7FFF : n;
}

void File::flush() {
  if (_file)
    _file->sync();
}

bool File::seek(uint32_t pos) {
  if (! _file) return false;

  return _file->seekSet(pos);
}

uint32_t File::position() {
  if (! mockFile.is_open()) return -1;
  return mockFile.tellp();
}

uint32_t File::size() {
    return _size;
}

void File::close() {
  if (mockFile.is_open())
      mockFile.close();
    
  if (_file) {
    _file->close();
    free(_file); 
    _file = 0;

    /* for debugging file open/close leaks
    nfilecount--;
    Serial.print("Deleted ");
    Serial.println(nfilecount, DEC);
    */
  }
}

File::operator bool() {
  if (_file) 
    return  _file->isOpen();
  return false;
}

