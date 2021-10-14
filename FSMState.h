#ifndef _FSM_STATE_H_
#define _FSM_STATE_H_

#include "World.h"
#include "States.h"

class FSMState {
public:

    FSMState() {
    }

    virtual ~FSMState() {

    }

    virtual States::ID getID() = 0;

    // The FSM sets the world before calling 'enter',
    // and sets it to null after calling 'exit'.
    void setWorld(World* w) {
        world = w;
    }

    World* getWorld() const {
        return world;
    }    

    virtual void enter(FSMState* previousState) {
        (void)previousState;
    }

    virtual void exit(FSMState* nextState) {
        (void)nextState;
    }

    virtual void update(uint32_t dt) {
        (void)dt;
    }

    virtual FSMState* updateTransitions() {
        return nullptr;
    }
    
private:
    World* world = nullptr;
};

#endif
