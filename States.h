#ifndef _STATES_H_
#define _STATES_H_

// Don't put FSMState subclass headers here!
// Forward declare them instead, so we can include
// this file in the FSMState subclass headers (to expose ID enum).

class PupilJoypadState;
class PupilAutomaticState;
class OutlineJoypadIdleState;
class OutlineJoypadBlinkState;
class OutlineJoypadGlareState;
class OutlineAutomaticBlinkState;

namespace States {

    enum class ID: uint8_t {
        // Pupil states
        pupilJoypad,
        pupilAutomatic,

        // Outline states
        outlineJoypadIdle,
        outlineJoypadBlink,
        outlineJoypadGlare,
        outlineAutomaticBlink,
    };

    extern PupilJoypadState* pupilJoypad;
    extern PupilAutomaticState* pupilAutomatic;

    extern OutlineJoypadIdleState* outlineJoypadIdle;
    extern OutlineJoypadBlinkState* outlineJoypadBlink;
    extern OutlineJoypadGlareState* outlineJoypadGlare;
    extern OutlineAutomaticBlinkState* outlineAutomaticBlink;
}

#endif
