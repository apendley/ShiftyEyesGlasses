#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_

#include <WiiChuck.h>
#include "WiiChuckButton.h"

class Joypad {
public:

    Joypad()
        : accessory()
        , zButton(&accessory, 10)
        , cButton(&accessory, 11)
    { 
    }

    bool begin() {
        accessory.begin();

        // Currently only support the nunchuck.
        if (accessory.type != NUNCHUCK) {
            return false;
        }

        // Attempt to read; if we fail, return false.
        if (!accessory.readData()) {
            return false;
        }

        zButton.begin();
        cButton.begin();

        isInitialized = true;

        return true;
    }

    void update() {
        if (!isInitialized) {
            return;
        }

        accessory.readData();
        zButton.update();
        cButton.update();
    }

    uint8_t getJoyX() const {
        return isInitialized ? accessory.values[0] : 255/2;
    }

    uint8_t getJoyY() const {
        // Invert so that down on the stick is down on the glasses.
        auto value = isInitialized ? accessory.values[1] : 255/2;
        return 255 - value;
    }

    DigitalInput const* getZButton() const {
        return &zButton;
    }

    DigitalInput const* getCButton() const {
        return &cButton;
    }

private:
    Accessory accessory;
    WiiChuckButton zButton;
    WiiChuckButton cButton;

    bool isInitialized = false;
};

#endif
