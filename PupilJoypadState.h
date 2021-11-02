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
        uint16_t joyX = 255 - (world->getJoypad()->getX() + 128);
    
        // Invert so that "down" matches down on the glasses.
        uint16_t joyY = 255 - (world->getJoypad()->getY() + 128);
    
        // bias joyY downward since there are fewer pixels on the bottom half
        joyY = min(joyY + 48, 255);
        
        int8_t pupilX = map(joyX, 0, 255, 1, 6);
        int8_t pupilY = map(joyY, 0, 255, 0, 3);
        world->getEyes()->setPupilPosition(pupilX, pupilY);
    }
};


#endif
