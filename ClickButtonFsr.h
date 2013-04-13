#ifndef ClickButton_H
#define ClickButton_H

#if (ARDUINO <  100)
#include <WProgram.h>
#else
#include <Arduino.h>
#endif


#define FSR_LOW 0
#define FSR_MIDDLE 1
#define FSR_HIGH 2

class ClickButtonFsr
{
  public:
    ClickButtonFsr(uint8_t buttonPin, int lowThreshold, int highThreshold);
    int Update();
    int click;                    // click code to return depending on button presses / clicks
    boolean depressed;            // the currently debounced button (press) state (presumably it is not sad :)
    int maxPresses;
    long debounceTime;
    long multiclickTime;
    long heldDownTime;

    int fsrValue;                 // voltage on the FSR pin
    byte btnFlick;             // Current appearant button state (which may flicker, or flick of a switch, etc..)
  private:
    uint8_t pin;                  // Arduino pin connected to the button
    boolean active;               // Type of button - active LOW or HIGH

    byte lastState;            // previous button reading
    int buttonPresses;            // Number of times button is pressed within CLICK_MULTICLICK_DELAY milliseconds
    long lastBounceTime;          // the last time the button input pin was toggled, due to noise or a press
    int lowThreshold;             // if the FSR voltage goes below this threshold, it will be considered active
    int highThreshold;            // if the FSR voltage goes above this threshold, it will be considered inactive
};

#endif

