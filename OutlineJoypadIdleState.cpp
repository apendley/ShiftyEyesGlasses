#include <Arduino.h>
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
    if (world->getJoypad()->getZButton()->rose()) {
        return States::outlineJoypadBlink;
    }
    else if (world->getJoypad()->getCButton()->rose()) {
        return States::outlineJoypadGlare;
    }

    return nullptr;	
}
