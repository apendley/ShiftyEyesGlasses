#include <Arduino.h>
#include "OutlineJoypadBlinkState.h"
#include "OutlineJoypadIdleState.h"

FSMState* OutlineJoypadBlinkState::updateTransitions() {
    auto world = getWorld();

    // Go until we get to the last eyelid position;
    if (world->getEyes()->getEyelidPosition() < Eyes::maxEyelidPosition) {
        return nullptr;
    }

    // Once there, if the z button is off, go back to idle.
    if (world->getJoypad()->getZButton()->isOff()) {
        return States::outlineJoypadIdle;
    }

    return nullptr;	
}
