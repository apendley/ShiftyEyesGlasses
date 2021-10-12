#ifndef _WIICHUCK_BUTTON_H_
#define _WIICHUCK_BUTTON_H_

#include <WiiChuck.h>
#include "DigitalInput.h"

class WiiChuckButton: public DigitalInput {
public:

    WiiChuckButton(Accessory* nunchuck, uint8_t buttonIndex)
      : nunchuck(nunchuck)
      , buttonIndex(buttonIndex)
    {
    }

    virtual ~WiiChuckButton() { }

    virtual int readInput() {
        return nunchuck->values[buttonIndex];
    }

protected:
    Accessory* nunchuck;
    uint8_t buttonIndex;
};

#endif
