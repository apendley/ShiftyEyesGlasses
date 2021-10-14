#ifndef _OUTLINE_JOYPAD_BLINK_STATE_H_
#define _OUTLINE_JOYPAD_BLINK_STATE_H_

#include "FSMState.h"
#include "World.h"

class OutlineJoypadBlinkState: public FSMState {
public:

    OutlineJoypadBlinkState() { }

    virtual ~OutlineJoypadBlinkState() { }

    virtual States::ID getID() {
        return States::ID::outlineJoypadBlink;
    }

    virtual void update(uint32_t) {
        auto world = getWorld();
        auto eyes = world->getEyes();

        auto eyelidPosition = eyes->getEyelidPosition();

        if (eyelidPosition < Eyes::maxEyelidPosition) {
            eyelidPosition += 1;
            eyes->setEyelidPosition(eyelidPosition);    
        } 
    }

    virtual FSMState* updateTransitions();
};


#endif
