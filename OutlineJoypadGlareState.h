#ifndef _OUTLINE_JOYPAD_GLARE_STATE_H_
#define _OUTLINE_JOYPAD_GLARE_STATE_H_

#include "FSMState.h"
#include "World.h"

class OutlineJoypadGlareState: public FSMState {
public:

    OutlineJoypadGlareState() { }

    virtual ~OutlineJoypadGlareState() { }

    virtual States::ID getID() {
        return States::ID::outlineJoypadGlare;
    }

    virtual void update(uint32_t) {
        auto world = getWorld();
        auto eyes = world->getEyes();

        auto eyelidPosition = eyes->getEyelidPosition();

        if (eyelidPosition < 2) {
            eyelidPosition += 1;
            eyes->setEyelidPosition(eyelidPosition);    
        } 
    }

    virtual FSMState* updateTransitions();
};


#endif
