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
#include "LoopTimer.h"
#include "Color.h"

Adafruit_EyeLights_buffered glasses;

// Update once every 16 ms, or about 60 fps.
const uint32_t updateInterval = 16;
LoopTimer loopTimer;
uint32_t elapsed = 0;

// Define colors and brightness.
static const uint8_t ringBrightness = 16;
const uint32_t ringColor = Color::color(255, 255, 0);
const uint16_t eyeColor = Color::color565(255, 0, 0);

// Variables for pupil animation.
int8_t pupilX = 3;
int8_t pupilY = 3;
int8_t nextPupilX = pupilX;
int8_t nextPupilY = pupilY;
int8_t dX = 0;
int8_t dY = 0;
uint16_t gazeFrames = 20;
uint16_t gazeCountdown = 100;

// Variables for blink animation.
uint8_t blinkHeight[] = {0, 1, 2, 3, 4, 5, 6, 6, 6, 5, 4, 3, 2, 1, 0};
const uint8_t blinkHeightFrames = sizeof(blinkHeight);
uint16_t blinkCountdown = 100;

void setup() {
    Serial.begin(115200);

    if (!glasses.begin(IS3741_ADDR_DEFAULT, &Wire)) {
        Serial.println("glasses not found");
        while(true);
    }

    Wire.setClock(800000); 

    // Set brightness to max and bring controller out of shutdown state
    glasses.setLEDscaling(0xFF);
    glasses.setGlobalCurrent(0xFF);
    glasses.enable(true);

    // Set ring brightness and rotation.
    glasses.setRotation(0);
    glasses.left_ring.setBrightness(ringBrightness);
    glasses.right_ring.setBrightness(ringBrightness);    

    // Clear glasses
    glasses.fill(0);

    // Start the loop timer.
    loopTimer.reset();
}

void loop() {
    // Update the loop timer and track how much time has elapsed since the last update.
    uint32_t dt = loopTimer.update();
    elapsed += dt;

    // Update interval has not elapsed, wait a bit and return.
    if (elapsed < updateInterval) {
        delay(1);
        return;
    }

    // Roll over
    elapsed %= updateInterval;

    // clear the display
    glasses.fill(0);

    // This is a pretty naive and brute force approach to drawing everything, and could definitely
    // be done more efficiently and with a lot less overdraw. The basic strategy here:
    // 1) Draw the rings
    glasses.left_ring.fill(ringColor);
    glasses.right_ring.fill(ringColor);

    // 2) Draw the pupils
    updatePupils();

    // 3) When blinking, clear the corresponding matrix and ring pixels down to 
    //    the "blink height" for each frame of the blink animation, drawing over
    //    the eye outline and pupil.
    // 4) Also, when blinking, draw a line representing the "eye lid"
    //    at the top of the "blink height", as long as that row is within the matrix.        
    updateBlink();

    // 5) Finally, flush the buffer to the leds.
    glasses.show();
}

bool isEyePositionValid(int8_t dx, int8_t dy) {
    return (dx * dx + dy * dy) >= 8;
}

void chooseNextPupilPosition() {
    // We've arrived at the next pupil position.
    pupilX = nextPupilX;
    pupilY = nextPupilY;        

    // Loop until we randomly choose an eye position within the circle.
    do {
        nextPupilX = random(6); 
        nextPupilY = random(4);
        dX = nextPupilX - 3;
        dY = nextPupilY - 2;
    } 
    while(isEyePositionValid(dX, dY));

    // Distance to next pupil position.
    dX = nextPupilX - pupilX;
    dY = nextPupilY - pupilY;

    // Duration of eye movement.
    gazeFrames = random(4, 20);

    // Count to end of next movement.
    gazeCountdown = random(gazeFrames, 120);
}

void updatePupils() {
    gazeCountdown -= 1;

    if (gazeCountdown <= gazeFrames) {
        // Eyes are moving, draw interpolated position.
        drawPupils(nextPupilX - (dX * gazeCountdown / gazeFrames),
                   nextPupilY - (dY * gazeCountdown / gazeFrames));

        if (gazeCountdown == 0) {
            chooseNextPupilPosition();
        }
    }
    else {
        // Eyes are stationary.
        drawPupils(pupilX, pupilY);
    }
}

void drawPupils(int8_t x, int8_t y) {
    glasses.fillRect(x, y, 2, 2, eyeColor);
    glasses.fillRect(x + 10, y, 2, 2, eyeColor);
}

void updateBlink() {
    blinkCountdown--;

    // The last frames of the countdown are using to animate the blink.
    if (blinkCountdown < blinkHeightFrames) {
        auto frame = blinkHeightFrames - 1 - blinkCountdown;

        // "Row 0". Clear the top 5 pixels of the rings, since they aren't in the matrix.
        const uint8_t topRow[] = {22, 23, 0, 1, 2};

        for (uint32_t i = 0; i < sizeof(topRow); i++) {
            glasses.left_ring.setPixelColor(topRow[i], 0);
            glasses.right_ring.setPixelColor(topRow[i], 0);
        }

        // Get the "blink height" for the frame.
        auto height = blinkHeight[frame];

        // Clear rows 1-5 of the matrix. This will also clear the shared ring pixels.
        if (height > 0) {
            auto fillHeight = min(height, 5);
            glasses.fillRect(0, 0, glasses.width(), fillHeight, 0);
        }

        // On row 1, we need to also clear a ring pixel that's not part of the matrix.
        if (height > 1) {
            const uint16_t pixelIndex = 4;
            glasses.left_ring.setPixelColor(pixelIndex, 0);
            glasses.right_ring.setPixelColor(glasses.right_ring.numPixels() - pixelIndex, 0);
        }

        // "Row 6" is the ring pixels just under the matrix.
        if (height > 5) {
            const uint8_t row[] = {8, 16};

            for (uint32_t i = 0; i < sizeof(row); i++) {
                glasses.left_ring.setPixelColor(row[i], 0);
                glasses.right_ring.setPixelColor(row[i], 0);
            }
        }

        // draw eyelid only up until the last row of the matrix.
        if (height <= 5) {
            auto c = Color::scale(ringColor, ringBrightness);
            glasses.fillRect(0, height, 7, 1, Color::color565(c));
            glasses.fillRect(11, height, 7, 1, Color::color565(c));
        }
    }

    // Done blinking, start a new countdown!
    if (blinkCountdown == 0) {
        blinkCountdown = random(20, 180);
    }    
}
