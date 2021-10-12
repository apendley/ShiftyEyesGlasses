#ifndef _DIGITAL_INPUT_H_
#define _DIGITAL_INPUT_H_

#include <Arduino.h>

class DigitalInput {
public:

    DigitalInput() { }

    virtual ~DigitalInput() { }

    virtual void setupInput() { }
    virtual int readInput() { return 0; }

    virtual void setup() {
        setupInput();
        state = lastState = debounceLastState = readInput();
        lastDebounceTime = 0;        
    }

    void update() {
        uint32_t now = millis();
        
        lastState = state;
        uint8_t newState = readInput();
        
        if (newState != debounceLastState) {
            lastDebounceTime = now;
        }
    
        uint32_t timeSinceLastDebounce = now - lastDebounceTime;
    
        if (timeSinceLastDebounce > debounceDuration) {
            if (newState != state) {
                state = newState;
            }
        }
    
        debounceLastState = newState;
    }

    bool changed() const {
        return state != lastState;
    }
    
    bool rose() const {
        return changed() && isOn();
    }
    
    bool fell() const {
        return changed() && isOff();
    }

    virtual bool isOn() const {
        return state;
    }

    bool isOff() const {
        return !isOn();
    }

private:
    static const uint32_t debounceDuration = 15;

    uint8_t state = 0;
    uint8_t lastState = 0;
    uint8_t debounceLastState = 0;
    uint32_t lastDebounceTime = 0;    
};

class DigitalPinInput: public DigitalInput {
public:

    enum class Mode: uint8_t {
        input = INPUT,
        pullup = INPUT_PULLUP,
        pulldown = INPUT_PULLDOWN
    };

    DigitalPinInput(uint8_t pin, Mode mode = Mode::pullup)
      : pin(pin)
      , mode(mode)
    {
    }

    virtual ~DigitalPinInput() { }

    virtual void setupInput() {
        pinMode(pin, uint8_t(mode));
    }

    virtual int readInput() {
        return digitalRead(pin);
    }

    virtual bool isOn() const {
        if (mode != Mode::pullup) {
            return isOn();
        }
        else {
            return isOff();
        }
    }        

    uint8_t getPin() const {
        return pin;
    }
    
    Mode getMode() const {
        return mode;
    }

protected:
    const uint8_t pin;
    const Mode mode;
};

#endif
