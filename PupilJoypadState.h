#ifndef _PUPIL_JOYPAD_STATE_H_
#define _PUPIL_JOYPAD_STATE_H_

#include "FSMState.h"
#include "World.h"

class PupilJoypadState: public FSMState {
public:

    PupilJoypadState() {
    }

    virtual ~PupilJoypadState() {
    }

    virtual States::ID getID() {
        return States::ID::pupilJoypad;
    }

    virtual void update(uint32_t) {
        auto world = getWorld();
        
        // Invert since "left" is opposite wearer's left if looking at glasses.
        int joyX = 255 - world->getJoypad()->getJoyX();
    
        // Invert so that "down" matches down on the glasses.
        int joyY = world->getJoypad()->getJoyY();
    
        // bias joyY downward since there are fewer pixels on the bottom half
        joyY = min(joyY + 48, 255);
        
        int8_t pupilX = map(joyX, 25, 230, 1, 6);
        int8_t pupilY = map(joyY, 25, 230, 0, 3);
        world->getEyes()->setPupilPosition(pupilX, pupilY);
    }
};


#endif
