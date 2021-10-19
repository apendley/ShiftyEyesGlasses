#ifndef _OUTLINE_AUTOMATIC_BLINK_STATE_H_
#define _OUTLINE_AUTOMATIC_BLINK_STATE_H_

#include "FSMState.h"
#include "World.h"

class OutlineAutomaticBlinkState: public FSMState {
public:

    OutlineAutomaticBlinkState() { }

    virtual ~OutlineAutomaticBlinkState() { }

    virtual States::ID getID() {
        return States::ID::outlineAutomaticBlink;
    }

    virtual void enter(FSMState*) {
        auto world = getWorld();
        auto eyes = world->getEyes();

        eyes->setEyelidPosition(0);
        blinkCountdown = random(20, 100);
    }

    virtual void update(uint32_t) {
        static const uint8_t blinkRowIndices[] = {1, 2, 3, 4, 5, 6, 7, 7, 7, 6, 5, 4, 3, 2, 1};    
        static const uint8_t numBlinkFrames = sizeof(blinkRowIndices);

        auto world = getWorld();

        blinkCountdown--;

       if (blinkCountdown < numBlinkFrames) {
            auto frameIndex = numBlinkFrames - 1 - blinkCountdown;
            auto blinkRowIndex = blinkRowIndices[frameIndex];
            world->getEyes()->setEyelidPosition(blinkRowIndex);
        } else {
            world->getEyes()->setEyelidPosition(0);
        }

        // Done blinking, start a new countdown!
        if (blinkCountdown == 0) {
            blinkCountdown = random(50, 180);
        }            
    }

private:

    uint32_t blinkCountdown;
};


#endif
