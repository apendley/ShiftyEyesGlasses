#ifndef _OUTLINE_JOYPAD_IDLE_STATE_H_
#define _OUTLINE_JOYPAD_IDLE_STATE_H_

#include "FSMState.h"
#include "World.h"

class OutlineJoypadIdleState: public FSMState {
public:

    OutlineJoypadIdleState() { }

    virtual ~OutlineJoypadIdleState() { }

    virtual States::ID getID() {
        return States::ID::outlineJoypadIdle;
    }

    virtual void update(uint32_t) {
        auto world = getWorld();
        auto eyes = world->getEyes();

        auto eyelidPosition = eyes->getEyelidPosition();

        if (eyelidPosition > 0) {
            eyelidPosition -= 1;
            eyes->setEyelidPosition(eyelidPosition);    
        } 
    }

    virtual FSMState* updateTransitions();
};


#endif
