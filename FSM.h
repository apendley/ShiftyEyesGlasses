#ifndef _FSM_H_
#define _FSM_H_

#include "World.h"
#include "FSMState.h"

class FSM {
public:

    FSM(World* world)
        : world(world)
    {
    }

    virtual ~FSM() {
    }

    void setState(FSMState* nextState) {
        FSMState* previousState = currentState;
    
        if (previousState != nullptr) {
            previousState->exit(nextState);
            previousState->setWorld(nullptr);
        }
    
        currentState = nextState;
    
        if (currentState != nullptr) {
            currentState->setWorld(world);
            currentState->enter(previousState);
        }
    }

    void update(uint32_t dt) {
        if (currentState == nullptr) {
            return;
        }
    
        currentState->update(dt);
    
        FSMState* nextState = currentState->updateTransitions();
    
        if (nextState) {
            setState(nextState);
        }
    }

    FSMState* getCurrentState() const {
        return currentState;
    }

private:

    World* world;
    FSMState* currentState = nullptr;
};

#endif
