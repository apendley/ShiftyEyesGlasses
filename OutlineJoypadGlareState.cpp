#include <Arduino.h>
#include "OutlineJoypadGlareState.h"
#include "OutlineJoypadIdleState.h"

FSMState* OutlineJoypadGlareState::updateTransitions() {
    auto world = getWorld();

    // If the z button is off, go back to idle.
    if (world->getJoypad()->getCButton()->isOff()) {
        return States::outlineJoypadIdle;
    }

    return nullptr;	
}
