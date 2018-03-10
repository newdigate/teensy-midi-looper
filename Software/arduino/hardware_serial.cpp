/*
  DSM2_tx implements the serial communication protocol used for operating
  the RF modules that can be found in many DSM2-compatible transmitters.

  Copyrigt (C) 2012  Erik Elmore <erik@ironsavior.net>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.  
*/

#include <cstring>
#include <iostream>
#include <iomanip>

#include "hardware_serial.h"

void HardwareSerial::begin(unsigned long speed) {
  return;
}

void HardwareSerial::end() {
  return;
}

int HardwareSerial::available(void) {
  return (!_inputBuffer.isEmpty());
}

void HardwareSerial::begin(unsigned long, uint8_t) {

}

int HardwareSerial::peek(void) {
  return 0;
}

int HardwareSerial::read(void) {
    int result;
    _inputBuffer.pop(&result);
    return result;
}

int HardwareSerial::availableForWrite(void) {
  return 0;
}

void HardwareSerial::flush(void) {

}

int HardwareSerial::write(uint8_t a) {
  // std::cout << a;
  return 0;
}

HardwareSerial Serial;
