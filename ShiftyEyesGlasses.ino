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

#include "World.h"
#include "LoopTimer.h"
#include "FSM.h"
#include "States.h"
#include "PupilJoypadState.h"
#include "PupilAutomaticState.h"
#include "OutlineJoypadIdleState.h"
#include "OutlineAutomaticBlinkState.h"


// The world.
World world;

// State machines to control the world
FSM pupilStateMachine(&world);
FSM outlineStateMachine(&world);

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

void setup() {
    Serial.begin(115200);
//    while(!Serial);

    auto setupErrorCode = world.begin();

    switch (setupErrorCode) {
        case SetupErrorCode::none:
            pupilStateMachine.setState(States::pupilJoypad);
            outlineStateMachine.setState(States::outlineJoypadIdle);
            break;

        case SetupErrorCode::glassesFailure:
            // Can't recover from this.
            while(!Serial);
            Serial.println("Failed to initialize glasses.");
            while(true);
            break;

        case SetupErrorCode::joypadFailure:
            pupilStateMachine.setState(States::pupilAutomatic);
            outlineStateMachine.setState(States::outlineAutomaticBlink);
            break;
    }

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

    world.getJoypad()->update();

    pupilStateMachine.update(dt);
    outlineStateMachine.update(dt);

    world.draw();
}
