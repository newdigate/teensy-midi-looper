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

#include <stdint.h>
#include <iostream>
#include "../common/MidiLoopSequencer.h"
#include "../common/TFTPianoDisplay.h"
#include "SD/SD.h"
#include "AdafruitTFTMock.h"
#include <unistd.h>

using namespace std;

void millis_test() {
  unsigned long start = millis();
  cout << "millis() test start: " << start << endl;
  while( millis() - start < 10000 ) {
	cout << millis() << endl;
	delay(1);
  }
  unsigned long end = millis();
  cout << "End of test - duration: " << end - start << "ms" << endl;
}

void delay_test() {
  unsigned long start = millis();
  cout << "delay() test start: " << start << endl;
  while( millis() - start < 10000 ) {
	cout << millis() << endl;
	delay(250);
  }
  unsigned long end = millis();
  cout << "End of test - duration: " << end - start << "ms" << endl;
}

void run_tests();
void run_tests2();

int main(int argc, char **argv){
	std::cout << "starting app...\n";
    initialize_mock_arduino();
    run_tests();
}

uint8_t tab[][3] = {
        {0x90, 0x76, 0x64},
        {0x80, 0x76, 0x64}
};

void run_tests() {

    std::string mockSDCardPath = std::string("/Users/nicnewdigate/Development/sdcard");
    SDClass::setSDCardFolderPath(mockSDCardPath);

    //millis_test();
    midi::MidiInterface<HardwareSerial> *midi = new midi::MidiInterface<HardwareSerial>(Serial);
    MidiLoopSequencer midiLoopSequencer(midi);
    midiLoopSequencer.initialize();

    midiLoopSequencer.onKeyChanged += [] (bool pressed, uint8_t key, uint8_t velocity, uint8_t channel) {
        printf("%s %#2x ch:%#2x vel:%2x\n", (pressed)? "On" : "Off", key, channel, velocity);
    };

    midiLoopSequencer.onPositionChanged += [] (SongPosition position) {
        printf("Song: %#5d \t %#5d\n", position.bar, position.beat);
    };

    int64_t t = 0;
    while (true) {

        //std::cin.read( Serial._inputBuffer.data(), 100);
        long milliseconds = millis();
        midiLoopSequencer.tick(milliseconds);

        usleep(10000);

        if (t % 50 == 10) {
            Serial._inputBuffer.push(tab[0] + 0);
            Serial._inputBuffer.push(tab[0] + 1);
            Serial._inputBuffer.push(tab[0] + 2);
        } else if (t % 50 == 25) {
            Serial._inputBuffer.push(tab[1] + 0);
            Serial._inputBuffer.push(tab[1] + 1);
            Serial._inputBuffer.push(tab[1] + 2);
        }
        t++;
    }
}

void run_tests2() {
    AdafruitTFTMock mock = AdafruitTFTMock(160, 160);
    TFTPianoDisplay tftPiano = TFTPianoDisplay(mock, 5, 0, 0, 0);
    tftPiano.drawPiano();
    mock.print("hello");
    string in;
    cin >> in;
}