# ShiftyEyesGlasses
Adaptation of the 'roboface' animation for the Adafruit IS31FL3741 LED Glasses. 

I borrowed the concept and a good bit of code from the 'roboface' example in the Adafruit LED backpack library (https://github.com/adafruit/Adafruit_LED_Backpack/) and adapted it to the nifty LED Glasses by Adafruit (https://www.adafruit.com/product/5210). 

This branch uses the Adafruit nRF52840 LED Glasses Driver's (https://www.adafruit.com/product/5217) BLE capabilities to become a BLE Central. It listens for a gamepad peripheral, and once connected, the gamepad can be used to control the pupil's movement using the left stick, as well as make the eyelids glare, close, and blink (using the gamepad's "z" and "c" buttons)! When not connected to a gamepad, the eyes will behave like the classic roboface example in the main branch.