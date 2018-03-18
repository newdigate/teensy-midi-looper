//
//  Adafruit_GFX_juce.hpp
//  TFTDisplayEmulator - App
//
//  Created by Nicholas Newdigate on 11/03/2018.
//  Copyright © 2018 ROLI Ltd. All rights reserved.
//

#ifndef Adafruit_GFX_juce_hpp
#define Adafruit_GFX_juce_hpp

#include <stdio.h>
#include <Adafruit/Adafruit_GFX.h>
#include "../JuceLibraryCode/JuceHeader.h"

class Adafruit_GFX_juce : public Adafruit_GFX {
public:
    inline Adafruit_GFX_juce(int16_t w, int16_t h, juce::Image &image) : Adafruit_GFX(w, h) {
        _image = &image;
    }
    
    inline int write(uint8_t c) {
        if(!gfxFont) { // 'Classic' built-in font
            
            if(c == '\n') {                        // Newline?
                cursor_x  = 0;                     // Reset x to zero,
                cursor_y += textsize * 8;          // advance y one line
            } else if(c != '\r') {                 // Ignore carriage returns
                if(wrap && ((cursor_x + textsize * 6) > _width)) { // Off right?
                    cursor_x  = 0;                 // Reset x to zero,
                    cursor_y += textsize * 8;      // advance y one line
                }
                drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize);
                cursor_x += textsize * 6;          // Advance x one char
            }
        }
        return 1;
    }
    
    inline int write(unsigned char const* s, unsigned long c) {
        for(unsigned long i = 0; i < c; i++) {
            char c = s[i];
            print(c);
        }
        return 0;
    }
    
    inline void drawPixel(int16_t x, int16_t y, uint16_t color) {
        unsigned r = (color & 0xF800) >> 11;
        unsigned g = (color & 0x07E0) >> 5;
        unsigned b = color & 0x001F;
        
        r = (r * 255) / 31;
        g = (g * 255) / 63;
        b = (b * 255) / 31;
        _image->setPixelAt(x, y, juce::Colour(r, g, b));
    }
    
    //inline void startWrite(void) {}
    //inline void writePixel(int16_t x, int16_t y, uint16_t color) {}
    //inline void writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {}
    //inline void writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {}
    //inline void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {}
    //inline void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {}
    //inline void endWrite(void) {}
    
    // CONTROL API
    // These MAY be overridden by the subclass to provide device-specific
    // optimized code.  Otherwise 'generic' versions are used.
    //inline void setRotation(uint8_t r) {}
    //inline void invertDisplay(bool i) {}
    
    // BASIC DRAW API
    // These MAY be overridden by the subclass to provide device-specific
    // optimized code.  Otherwise 'generic' versions are used.
    //inline void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {}
    //inline void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {}
    //inline void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {}

    //inline void fillScreen(uint16_t color) {}
    // Optional and probably not necessary to change
    //inline void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {}
    //inline void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {}
private:
    juce::Image *_image;
};

#endif /* Adafruit_GFX_juce_hpp */
