#ifndef _JOYPAD_H_
#define _JOYPAD_H_

class Joypad {
public:

    Joypad() { }

    void update(int8_t _x, int8_t _y, uint32_t _buttons) {
        lastX = x;
        x = _x;

        lastY = y;
        y = _y;

        lastButtons = buttons;
        buttons = _buttons;
    }

    void reset() {
        x = lastX = 0;
        y = lastY = 0;
        buttons = lastButtons = 0;
    }

    bool isButtonDown(uint32_t b) {
        return buttons & b;
    }
    
    bool isButtonUp(uint32_t b) {
        return ~buttons & b;
    }
    
    bool buttonHeld(uint32_t b) {
        return (buttons & lastButtons) & b;
    }
    
    bool buttonFell(uint32_t b) {
        return (~buttons & lastButtons) & b;
    }
    
    bool buttonRose(uint32_t b) {
        return (buttons& ~lastButtons) & b;
    }
    
    bool buttonChanged(uint32_t b) {
        return (buttons ^ lastButtons) & b; 
    }

    int8_t getX() const {
        return x;
    }

    int8_t getLastX() const {
        return lastX;
    }    

    int8_t getY() const {
        return y;
    }

    int8_t getLastY() const {
        return lastY;
    }

    bool stickChanged() const {
        return (x != lastX) || (y != lastY);
    }

private:
    int8_t x = 0;
    int8_t lastX = 0;
    int8_t y = 0;
    int8_t lastY = 0;
    uint32_t buttons = 0;
    uint32_t lastButtons = 0;

};

#endif
