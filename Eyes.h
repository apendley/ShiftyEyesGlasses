#ifndef _EYES_H_
#define _EYES_H_

#include "Color.h"

class Eyes {
public:

    static const uint8_t numEyelidPositions = 8;
    static const uint8_t maxEyelidPosition = numEyelidPositions - 1;

    Eyes() {  }

    int8_t getPupilX() const {
        return pupilX;
    }

    void setPupilX(int8_t x) {
        pupilX = x;
    }

    int8_t getPupilY() {
        return pupilY;
    }    

    void setPupilY(int8_t y) {
        pupilY = y;
    }

    void setPupilPosition(int8_t x, int8_t y) {
        setPupilX(x);
        setPupilY(y);
    }

    uint8_t getEyelidPosition() const {
        return eyelidPosition;
    }

    void setEyelidPosition(uint8_t pos) {
        eyelidPosition = min(pos, maxEyelidPosition);
    }

    void setPupilColor(uint32_t rgb) {
        pupilColor = rgb;
    }

    uint32_t getPupilColor() const {
        return pupilColor;
    }

    uint8_t getOutlineBrightness() const {
        return outlineBrightness;
    }

    void setOutlineBrigntess(uint8_t b) {
        outlineBrightness = b;
    }

    uint32_t getOutlineColor() const {
        return outlineColor;
    }

    void setOutlineColor(uint32_t rgb) {
        outlineColor = rgb;
    }

    uint16_t getMatrixOutlineColor() const {
        auto scaled = Color::scale(outlineColor, outlineBrightness);
        return Color::rgb565(scaled);
    }
    
private:
    int8_t pupilX = 3;
    int8_t pupilY = 3;
    uint8_t eyelidPosition = 0; // 0 = fully open, maxEyelidPosition = fully closed
    uint32_t pupilColor = Color::rgb(255, 0, 0);
    uint8_t outlineBrightness = 16;
    uint32_t outlineColor = Color::rgb(255, 255, 0);
};

#endif
