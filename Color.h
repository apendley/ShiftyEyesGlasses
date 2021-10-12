// Copyright 2021 Aaron Pendley
//
// Permission is hereby granted, free of charge, to any person obtaining a 
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the 
// Software is furnished to do so, subject to the following conditions:
// The above copyright notice and this permission notice shall be included 
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

#ifndef _COLOR_H_
#define _COLOR_H_

#include <Adafruit_IS31FL3741.h>

namespace Color {

    inline uint32_t rgb(uint8_t r, uint8_t g, uint8_t b) {
        return Adafruit_IS31FL3741::Color(r, g, b);
    }

    inline uint16_t rgb565(uint32_t color888) {
        return Adafruit_IS31FL3741::color565(color888);
    }

    inline uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b) {
        return Adafruit_IS31FL3741::color565(r, g, b);
    }    

    inline uint32_t hsv(uint16_t hue, uint8_t sat = 255, uint8_t val = 255) {
        return Adafruit_IS31FL3741::ColorHSV(hue, sat, val);
    }

    inline uint16_t hsv565(uint16_t hue, uint8_t sat = 255, uint8_t val = 255) {
        return rgb565(hsv(hue, sat, val));
    }    

    inline uint32_t scale(uint32_t color, uint8_t brightness) {
        uint8_t r = (uint8_t)(color >> 16);
        r = (r * brightness) / 255;
        
        uint8_t g = (uint8_t)(color >> 8);
        g = (g * brightness) / 255;
        
        uint8_t b = (uint8_t)color;
        b = (b * brightness) / 255;

        return Color::rgb(r, g, b);
    }
}

#endif
