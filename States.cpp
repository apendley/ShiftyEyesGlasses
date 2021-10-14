#include <Arduino.h>
#include "States.h"

#include "PupilJoypadState.h"
#include "OutlineJoypadIdleState.h"
#include "PupilAutomaticState.h"
#include "OutlineAutomaticBlinkState.h"
#include "OutlineJoypadBlinkState.h"
#include "OutlineJoypadGlareState.h"


#define DECLARE_STATE(StateType, stateName)     \
    StateType stateName##State;                 \
    StateType* stateName = &stateName##State

namespace States {

    // Pupil states
    DECLARE_STATE(PupilJoypadState, pupilJoypad);
    DECLARE_STATE(PupilAutomaticState, pupilAutomatic);

    // Outline states
    DECLARE_STATE(OutlineJoypadIdleState, outlineJoypadIdle);
    DECLARE_STATE(OutlineJoypadBlinkState, outlineJoypadBlink);
    DECLARE_STATE(OutlineJoypadGlareState, outlineJoypadGlare);
    DECLARE_STATE(OutlineAutomaticBlinkState, outlineAutomaticBlink);
}
