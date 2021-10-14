#ifndef _PUPIL_AUTOMATIC_STATE_H_
#define _PUPIL_AUTOMATIC_STATE_H_

#include "FSMState.h"
#include "World.h"

class PupilAutomaticState: public FSMState {
public:

    PupilAutomaticState() { }

    virtual ~PupilAutomaticState() { }

    virtual States::ID getID() {
        return States::ID::pupilAutomatic;
    }

    virtual void enter(FSMState*) {
        auto world = getWorld();

        pupilX = nextPupilX = world->getEyes()->getPupilX();
        pupilY = nextPupilY = world->getEyes()->getPupilY();
        dX = 0;
        dY = 0;
        gazeFrames = 80;
        gazeCountdown = 100;
    }

    virtual void update(uint32_t) {
        auto world = getWorld();

        gazeCountdown -= 1;

        if (gazeCountdown <= gazeFrames) {
            // Pupils are moving, set interpolated position.
            world->getEyes()->setPupilX(nextPupilX - (dX * gazeCountdown / gazeFrames));
            world->getEyes()->setPupilY(nextPupilY - (dY * gazeCountdown / gazeFrames));

            if (gazeCountdown == 0) {
                chooseNextPupilPosition();
            }
        }
        else {
            // Pupils are stationary.
            world->getEyes()->setPupilPosition(pupilX, pupilY);
        }
    }

private:

    bool isEyePositionValid(int8_t dx, int8_t dy) {
        return (dx * dx + dy * dy) >= 8;
    }

    void chooseNextPupilPosition() {
        // We've arrived at the next pupil position.
        pupilX = nextPupilX;
        pupilY = nextPupilY;        

        // Loop until we randomly choose an eye position within the circle.
        do {
            nextPupilX = random(6); 
            nextPupilY = random(4);
            dX = nextPupilX - 3;
            dY = nextPupilY - 2;
        } 
        while(isEyePositionValid(dX, dY));

        // Distance to next pupil position.
        dX = nextPupilX - pupilX;
        dY = nextPupilY - pupilY;

        // Duration of eye movement.
        gazeFrames = random(4, 20);

        // Count to end of next pupil movement.
        gazeCountdown = random(gazeFrames, 120);
    }

private:

    int8_t pupilX;
    int8_t pupilY;
    int8_t nextPupilX;
    int8_t nextPupilY;
    int8_t dX;
    int8_t dY;
    uint16_t gazeFrames;
    uint16_t gazeCountdown;
};


#endif
