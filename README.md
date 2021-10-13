# ShiftyEyesGlasses
Adaptation of the 'roboface' animation for the Adafruit IS31FL3741 LED Glasses. 

I borrowed the concept and a good bit of code from the 'roboface' example in the Adafruit LED backpack library (https://github.com/adafruit/Adafruit_LED_Backpack/) and adapted it to the nifty LED Glasses by Adafruit (https://www.adafruit.com/product/5210). 

This branch takes advantage of the Feather RP2040's (https://www.adafruit.com/product/4884) dual I2C busses to enable using the WiiChuck breakout board and Wii Nunchuck to control the pupil movement and blinking. Even though I was able to get both devices working on the same bus, I still chose to use a bus for each device, as the nunchuck does not seem to like being clocked to speeds higher than 400KHz, but limiting the glasses to that rate means a *really slow* draw rate. This compromize allows us to maintain fast drawing. Soon, I'm going to merge the automatic and nunchuck-controlled functionality into the same firmware, but for now they're separate until I figure out exactly how I want to do that. 
