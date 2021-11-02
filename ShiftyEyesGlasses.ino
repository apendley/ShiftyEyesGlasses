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

#include <bluefruit.h>
#include "BLEClientHIDGamepad.h"
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

// The gamepad client.
BLEClientHidGamepad hid;

void setup() {
    Serial.begin(115200);
    //while(!Serial) delay(10);

    auto setupErrorCode = world.begin();

    switch (setupErrorCode) {
        case SetupErrorCode::none:
            pupilStateMachine.setState(States::pupilAutomatic);
            outlineStateMachine.setState(States::outlineAutomaticBlink);
            break;

        case SetupErrorCode::glassesFailure:
            // Can't recover from this.
            while(!Serial);
            Serial.println("Failed to initialize glasses.");
            while(true);
            break;
    }

    setupBLE();

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
    
    updateBleGamepad();
    pupilStateMachine.update(dt);
    outlineStateMachine.update(dt);

    world.draw();
}

void setupBLE() {
    Bluefruit.begin(0, 1);
    Bluefruit.Central.clearBonds();  
    Bluefruit.setName("Shifty Eyes");
    
    hid.begin();
  
    Bluefruit.setConnLedInterval(250);

    Bluefruit.Central.setConnectCallback(connect_callback);
    Bluefruit.Central.setDisconnectCallback(disconnect_callback);
    Bluefruit.Security.setSecuredCallback(connection_secured_callback);

    Bluefruit.Scanner.setRxCallback(scan_callback);
    Bluefruit.Scanner.restartOnDisconnect(true);
    Bluefruit.Scanner.setInterval(160, 80); // in unit of 0.625 ms
    Bluefruit.Scanner.filterService(hid);   // only report HID service
    Bluefruit.Scanner.useActiveScan(false);
    Bluefruit.Scanner.start(0);             // 0 = Don't stop scanning after n seconds    
}

void updateBleGamepad() { 
    if (!hid.discovered()) {
        return;
    }
  
    hid_gamepad_report_t report;
    hid.getGamepadReport(&report);
    gamepad_report_callback(&report);
}

void scan_callback(ble_gap_evt_adv_report_t* report) {
    uint8_t buffer[32];
    memset(buffer, 0, sizeof(buffer));    

    if(Bluefruit.Scanner.parseReportByType(report, BLE_GAP_AD_TYPE_COMPLETE_LOCAL_NAME, buffer, sizeof(buffer))) {
        Serial.printf("%s %s\n", "Connecting to:", buffer);
    }

    Bluefruit.Central.connect(report);
}

void connect_callback(uint16_t conn_handle) {
    BLEConnection* conn = Bluefruit.Connection(conn_handle);

    Serial.println("Connected");
    Serial.print("Discovering HID Service ... ");

    if (hid.discover(conn_handle)) {
        Serial.println("Found gamepad!");

        // HID device mostly require pairing/bonding
        conn->requestPairing();
    } else {
        Serial.println("No gamepad found.");
    
        // Disconnect since we couldn't find hid service
        conn->disconnect();
    }
}

void connection_secured_callback(uint16_t conn_handle) {
    BLEConnection* conn = Bluefruit.Connection(conn_handle);

    if (!conn->secured()) {
        // It is possible that connection is still not secured by this time.
        // This happens (central only) when we try to encrypt connection using stored bond keys
        // but peer reject it (probably it remove its stored key).
        // Therefore we will request an pairing again --> callback again when encrypted
        conn->requestPairing();
    } else {
        Serial.println("Connection secured");

        if (hid.gamepadPresent()) {
            hid.enableGamepad();
            Serial.println("Gamepad enabled!");

            pupilStateMachine.setState(States::pupilJoypad);
            outlineStateMachine.setState(States::outlineJoypadIdle);            
        }
    }
}

void disconnect_callback(uint16_t conn_handle, uint8_t reason) {
    (void) conn_handle;
    (void) reason;
  
    Serial.print("Disconnected, reason = 0x"); Serial.println(reason, HEX);

    pupilStateMachine.setState(States::pupilAutomatic);
    outlineStateMachine.setState(States::outlineAutomaticBlink);    
    world.getJoypad()->reset();
}

void gamepad_report_callback(hid_gamepad_report_t* report) {
    world.getJoypad()->update(report->x, report->y, report->buttons);
}
