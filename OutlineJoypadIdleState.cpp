#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include "OutlineJoypadIdleState.h"
#include "OutlineJoypadBlinkState.h"
#include "OutlineJoypadGlareState.h"

FSMState* OutlineJoypadIdleState::updateTransitions() {
    auto world = getWorld();

    // Wait until almost back up before allowing transitions
    if (world->getEyes()->getEyelidPosition() > 1) {
        return nullptr;
    }

    // If the z button rose, blink
    if (world->getJoypad()->buttonRose(GAMEPAD_BUTTON_Z)) {
        return States::outlineJoypadBlink;
    }
    else if (world->getJoypad()->buttonRose(GAMEPAD_BUTTON_C)) {
        return States::outlineJoypadGlare;
    }

    return nullptr;	
}
