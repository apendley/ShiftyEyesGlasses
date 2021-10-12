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

//
// Shifty Eyes Glasses
//
// I borrowed the concept and a good bit of code from the 'roboface' example in the
// Adafruit LED backpack library (https://github.com/adafruit/Adafruit_LED_Backpack/)
// and adapted it to the nifty LED Glasses by Adafruit (https://www.adafruit.com/product/5210).
// I used the Adafruit nRF52840 LED Glasses Driver (https://www.adafruit.com/product/5217),
// but this code should work on most microcontrollers with little or no adjustment (though on 
// slower microcontrollers you may want to increase the update interval until the animation is smooth).

#include <Adafruit_IS31FL3741.h>
#include <WiiChuck.h>
#include "WiiChuckButton.h"
#include "Rings.h"
#include "LoopTimer.h"
#include "Color.h"

Adafruit_EyeLights_buffered glasses;

Accessory nunchuck(&Wire);

// Simple helper class to help keep track of elapsed time.
LoopTimer loopTimer;

// Animate once every 16 ms, or about 60 fps.
// You want this number to at least be as high as the amount of time it takes 
// to update the animations plus draw the pixels to the glasses. On the
// glasses driver, it takes about 10-11 ms, so we can easily achieve ~60 fps.
// I like this better than using a delay because it allows for some variablility
// in logic and animation updates before the animation appears to "lag".
// You can also just slow this down if you prefer a slower animation rate.
const uint32_t animationInterval = 16;
uint32_t animationElapsed = 0;

// Pupil animation.
// This is basically the same as the 'roboface' sample with some tiny adjustments.
const uint16_t pupilColor = Color::rgb565(255, 0, 0);
int8_t pupilX = 3;
int8_t pupilY = 3;

// Blink animation
static const uint8_t eyeOutlineBrightness = 16;
const uint32_t ringEyeOutlineColor = Color::rgb(255, 255, 0);

// Convert the ring blink color and brightness to a matrix color value.
// Note that these LEDs lose color resolution on the low end, so the colors might
// not look how you expect linear interpolation like this.
const uint16_t matrixEyeOutlineColor = Color::rgb565(Color::scale(ringEyeOutlineColor, eyeOutlineBrightness));

// The blink countdown doubles as an "open eye state" timer,
// as well as an animation frame counter.
uint8_t blinkCountdown = 255;

// These are the "Frames" of animation for the rings when blinking.
// Each value corresponds to the top line of the blink, or the "eyelid".
// On that line, we draw the eye outline on both the matrix and the rings.
// Above that line, we clear the pixels by drawing "black", giving the 
// appearance of blinking.
const uint8_t blinkRowIndices[] = {1, 2, 3, 4, 5, 6, 7, 7, 7, 6, 5, 4, 3, 2, 1};
const uint8_t numBlinkFrames = sizeof(blinkRowIndices);

WiiChuckButton zButton(&nunchuck, 10);
WiiChuckButton cButton(&nunchuck, 11);

void setup() {
    Serial.begin(115200);
//    while(!Serial);

    Wire.setClock(400000);
    Wire1.setClock(800000);

    nunchuck.begin();
    if (nunchuck.type == Unknown) {
        nunchuck.type = NUNCHUCK;
    }

    zButton.setup();
    cButton.setup();    

    if (!glasses.begin(IS3741_ADDR_DEFAULT, &Wire1)) {
        Serial.println("glasses not found");
        while(true);
    }

    // Set brightness to max and bring controller out of shutdown state
    glasses.setLEDscaling(0xFF);
    glasses.setGlobalCurrent(0xFF);
    glasses.enable(true);

    // Set ring brightness and rotation.
    glasses.setRotation(0);
    glasses.left_ring.setBrightness(eyeOutlineBrightness);
    glasses.right_ring.setBrightness(eyeOutlineBrightness);    

    // Clear glasses
    glasses.fill(0);

    // Start the loop timer.
    loopTimer.reset();
}

void loop() {
    // Update the loop timer and track how much time has elapsed since the last update.
    uint32_t dt = loopTimer.update();
    animationElapsed += dt;

    // Animation interval has not elapsed, wait a bit and return.
    if (animationElapsed < animationInterval) {
        delay(1);
        return;
    }

    // Roll over, using modulus in case "drop" some frames.
    animationElapsed %= animationInterval;

    // Read the nunchuck
    nunchuck.readData();
    zButton.update();
    cButton.update();

    // clear the display
    glasses.fill(0);

    // 1) Update and draw the pupils. We do this first
    // so the "blink" animation will draw over the
    // pupils, making them appear to go behind the eyelids.
    updatePupils();

    // 2) Update the blink animation.
    updateBlink();

    // 3) Show our pretty eyes.
    glasses.show();
}

bool isEyePositionValid(int8_t dx, int8_t dy) {
    return (dx * dx + dy * dy) >= 8;
}

void updatePupils() {
    // Invert since "left" is opposite wearer's left if looking at glasses.
    int joyX = 255 - nunchuck.values[0];

    // Invert so that "down" matches.
    int joyY = 255 - nunchuck.values[1];

    // bias joyY downward since there are fewer pixels on the bottom half
    joyY = min(joyY + 48, 255);
    
    pupilX = map(joyX, 25, 230, 1, 6);
    pupilY = map(joyY, 25, 230, 0, 3);
    drawPupils(pupilX, pupilY);    
}

void drawPupils(int8_t x, int8_t y) {
    glasses.fillRect(x, y, 2, 2, pupilColor);
    glasses.fillRect(x + 10, y, 2, 2, pupilColor);
}

void drawRingRow(uint8_t index, uint32_t color) {
    auto ringRow = Rings::getRow(index);
    
    for (uint8_t i = 0; i < ringRow->numIndices; i++) {
        glasses.left_ring.setPixelColor(ringRow->indices[i], color);
        glasses.right_ring.setPixelColor(ringRow->indices[i], color);
    }    
}

void updateBlink() {
    if (blinkCountdown == 255) {
        if (zButton.rose()) {
            blinkCountdown = numBlinkFrames;
        }       
    } else {
        if (blinkCountdown > 0) {
            blinkCountdown--;
        } else {
            blinkCountdown = 255;
        }
    }

    // The last frames of the countdown are using to animate the blink.
    if (blinkCountdown < numBlinkFrames) {
        auto frameIndex = numBlinkFrames - 1 - blinkCountdown;
        auto blinkRowIndex = blinkRowIndices[frameIndex];

        // Draw the matrix pixels. We're drawing first so the ring drawing
        // can have priority over the overlapping matrix pixels.
        // The matrix's first row is actually the ring's second "row",
        // so we can just draw zeros over the "eye" pixels,
        // except on the line where we want to draw the eyelid
        for (int i = 0; i < glasses.height(); i++) {
            int matrixRowIndex = blinkRowIndex - 1;
            
            if (matrixRowIndex >= 0) {
                if (i == matrixRowIndex) {
                    // Left eye
                    glasses.fillRect(1, matrixRowIndex, 6, 1, matrixEyeOutlineColor);
                    // Right eye
                    glasses.fillRect(11, matrixRowIndex, 6, 1, matrixEyeOutlineColor);
                }
                else if (i < matrixRowIndex) {
                    // Left eye
                    glasses.fillRect(1, i, 6, 1, 0);
                    // Right eye
                    glasses.fillRect(11, i, 6, 1, 0);
                }
            }
        }

        // Draw the rings now, clearing the rows above the "blink row".
        for (int i = 0; i < Rings::numRows; i++) {
            if (i < blinkRowIndex) {
                drawRingRow(i, 0);
            } else {
                drawRingRow(i, ringEyeOutlineColor);
            }
        }
    } else {
        // We're not blinking, draw all of the ring pixels.
        glasses.left_ring.fill(ringEyeOutlineColor);
        glasses.right_ring.fill(ringEyeOutlineColor);
    }
}
