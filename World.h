#ifndef _WORLD_H_
#define _WORLD_H_

#include "Eyes.h"
#include <Adafruit_IS31FL3741.h>
#include "Joypad.h"
#include "Rings.h"

enum class SetupErrorCode: uint8_t {
    none = 0,
    glassesFailure
};

class World {
public:

    World() { }

    // Returns 0 if no issue, otherwise an error code
    SetupErrorCode begin() {
        if (!glasses.begin(IS3741_ADDR_DEFAULT)) {
            return SetupErrorCode::glassesFailure;
        }

        // Set brightness to max and bring controller out of shutdown state
        glasses.setLEDscaling(0xFF);
        glasses.setGlobalCurrent(0xFF);
        glasses.enable(true);
    
        // Set ring brightness and rotation.
        glasses.setRotation(0);
    
        // Clear glasses
        glasses.fill(0);

        return SetupErrorCode::none;
    }

    void draw() {
        glasses.fill(0);
        drawPupils();
        drawEyeOutlines();
        glasses.show();
    }

    Eyes* getEyes() {
        return &eyes;
    }

    Joypad* getJoypad() {
        return &joypad;
    }

    Adafruit_EyeLights_buffered* getGlasses() {
        return &glasses;
    }

private:
    void drawPupils() {
        int8_t x = eyes.getPupilX();
        int8_t y = eyes.getPupilY();
        uint16_t c = Color::rgb565(eyes.getPupilColor());
        
        glasses.fillRect(x, y, 2, 2, c);
        glasses.fillRect(x + 10, y, 2, 2, c);
    }
    
    void drawRingRow(uint8_t index, uint32_t color) {
        auto ringRow = Rings::getRow(index);
        
        for (uint8_t i = 0; i < ringRow->numIndices; i++) {
            glasses.left_ring.setPixelColor(ringRow->indices[i], color);
            glasses.right_ring.setPixelColor(ringRow->indices[i], color);
        }    
    }
    
    void drawEyeOutlines() {
        auto eyelidPosition = eyes.getEyelidPosition();

        glasses.left_ring.setBrightness(eyes.getOutlineBrightness());
        glasses.right_ring.setBrightness(eyes.getOutlineBrightness());            
    
        if (eyelidPosition == 0) {
            glasses.left_ring.fill(eyes.getOutlineColor());
            glasses.right_ring.fill(eyes.getOutlineColor());
        } else {
            // Draw the matrix pixels. We're drawing first so the ring drawing
            // can have priority over the overlapping matrix pixels.
            // The matrix's first row is actually the ring's second "row",
            // so we can just draw zeros over the "eye" pixels,
            // except on the line where we want to draw the eyelid
            for (int i = 0; i < glasses.height(); i++) {
                int matrixRowIndex = eyelidPosition - 1;
                
                if (matrixRowIndex >= 0) {
                    if (i == matrixRowIndex) {
                        // Left eye
                        glasses.fillRect(1, matrixRowIndex, 6, 1, eyes.getMatrixOutlineColor());
                        // Right eye
                        glasses.fillRect(11, matrixRowIndex, 6, 1, eyes.getMatrixOutlineColor());
                    }
                    else if (i < matrixRowIndex) {
                        // Left eye
                        glasses.fillRect(1, i, 6, 1, 0);
                        // Right eye
                        glasses.fillRect(11, i, 6, 1, 0);
                    }
                }
            }
    
            // Draw the rings now, clearing the rows above the "blink row".
            for (int i = 0; i < Rings::numRows; i++) {
                if (i < eyelidPosition) {
                    drawRingRow(i, 0);
                } else {
                    drawRingRow(i, eyes.getOutlineColor());
                }
            }        
        }
    }    

private:
    
    Adafruit_EyeLights_buffered glasses;
    Joypad joypad;
    Eyes eyes;
};

#endif
