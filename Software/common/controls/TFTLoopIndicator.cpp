//
// Created by Nicholas Newdigate on 07/04/2018.
// https://forum.arduino.cc/index.php?topic=413186.0

#include <cmath>
#include "TFTLoopIndicator.h"

void TFTLoopIndicator::update(unsigned long millis) {
    if (_needs_refresh) {
    //Serial.print(".");
    //Serial.flush();
        //std::cout << (uint16_t)_track->_loop_phase << "\n";
        uint16_t segments = (_track._loop_phase * _width ) / 256;
        //_tft->fillRect( _x, _y, segments, _height, _indicatorOnColor );
        //_tft->fillRect( _x + segments + 1, _y, _width-segments, _height, _indicatorOffColor );
        uint16_t segments_360 = (_track._loop_phase * 360 ) / 256;
        uint16_t segments_120 = (_track._loop_phase * 120 ) / 256;
       
        //Serial.printf("%d\n", (unsigned long)segments );
       

        
        if (_lastSegment > segments_120)
            fillArc2(_x, _y, 0, 120, 12, 12, 2, _indicatorOffColor );
        else
            if (segments_120 - _lastSegment < 2)
                return;
        
        fillArc2(_x, _y, _lastSegment*3, segments_120-_lastSegment, 12, 12, 2, _indicatorOnColor );
        _lastSegment = segments_120;
        _needs_refresh = false;
    }
}

// #########################################################################
// Draw a circular or elliptical arc with a defined thickness
// #########################################################################

// x,y == coords of centre of arc
// start_angle = 0 - 359
// seg_count = number of 3 degree segments to draw (120 => 360 degree arc)
// rx = x axis radius
// yx = y axis radius
// w  = width (thickness) of arc in pixels
// colour = 16 bit colour value
// Note if rx and ry are the same then an arc of a circle is drawn
#define DEG2RAD 0.0174532925

int TFTLoopIndicator::fillArc2(int x, int y, int start_angle, int seg_count, int rx, int ry, int w, unsigned int colour)
{

    byte seg = 3; // Segments are 3 degrees wide = 120 segments for 360 degrees
    byte inc = 3; // Draw segments every 3 degrees, increase to 6 for segmented ring

    // Calculate first pair of coordinates for segment start
    float sx = cos((start_angle - 90) * DEG2RAD);
    float sy = sin((start_angle - 90) * DEG2RAD);
    uint16_t x0 = sx * (rx - w) + x;
    uint16_t y0 = sy * (ry - w) + y;
    uint16_t x1 = sx * rx + x;
    uint16_t y1 = sy * ry + y;

    // Draw colour blocks every inc degrees
    for (int i = start_angle; i < start_angle + seg * seg_count; i += inc) {

        // Calculate pair of coordinates for segment end
        float sx2 = cos((i + seg - 90) * DEG2RAD);
        float sy2 = sin((i + seg - 90) * DEG2RAD);
        int x2 = sx2 * (rx - w) + x;
        int y2 = sy2 * (ry - w) + y;
        int x3 = sx2 * rx + x;
        int y3 = sy2 * ry + y;

        _tft->fillTriangle(x0, y0, x1, y1, x2, y2, colour);
        _tft->fillTriangle(x1, y1, x2, y2, x3, y3, colour);

        // Copy segment end to sgement start for next segment
        x0 = x2;
        y0 = y2;
        x1 = x3;
        y1 = y3;
    }
    return 0;
}
