#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include "OutlineJoypadGlareState.h"
#include "OutlineJoypadIdleState.h"

FSMState* OutlineJoypadGlareState::updateTransitions() {
    auto world = getWorld();

    // If the C button is off, go back to idle.
    if (world->getJoypad()->isButtonUp(GAMEPAD_BUTTON_C)) {
        return States::outlineJoypadIdle;
    }

    return nullptr;	
}
