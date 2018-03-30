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
int main(int argc, const char * argv[]) {
    initialize_mock_arduino();
    SD.begin();
    bool exists = SD.exists("hello.txt");
    if (exists){
        File f = SD.open("hello.txt");
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
    return 0;
}
