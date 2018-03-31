//
//  main.c
//  TestSDReadWrite
//
//  Created by John Smith on 30/03/2018.
//  Copyright © 2018 John Smith. All rights reserved.
//

#include <stdio.h>
//#include "hardware_serial.h"
#include "../../../arduino/mock_arduino.h"
#include "../../../arduino/SD/SD.h"
void testWriteRead();

int main(int argc, const char * argv[]) {
    initialize_mock_arduino();
    SDClass::setSDCardFolderPath("/Users/johnsmith/Development/sdcard");
    SD.begin();
    bool exists = SD.exists("hello.txt");
    if (exists){
        File f = SD.open("hello.txt", O_READ);
        while (f.available()) {
            int x = f.read();
            char a = x >> 8;
            char b = x & 0xff;
            cout<< a << b;
        }
        f.close();
    }
        
    // insert code here...
    printf("Hello, World!\n");
    
    testWriteRead();
    
    return 0;
}

void testWriteRead() {
    const char *filename = "test.txt";
    if (SD.exists(filename))
        SD.rmdir(filename);
    
    File f = SD.open(filename, O_WRITE);
    for (uint8_t i=0; i<255; i++)
        f.write(i);
    f.flush();
    f.close();
    
    f = SD.open(filename, O_READ);
    for (uint8_t i=0; i<255; i++) {
        int j = f.read();
        char c = (char)j;
        cout << c;
    }
    f.close();
}
