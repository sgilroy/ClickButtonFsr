/*    ClickButtonFsr
 
 Arduino library that decodes multiple presses on an force sensitive
 resistor (FSR) as if it were a button.
 It returns a function code according to clicks (doubleclicks etc.).
 Also copes with buttons held down for a time.
 
 Based on ClickButton library by raronzen@gmail.com found at https://code.google.com/p/clickbutton/
 
 Returned click codes:

   A positive number denotes the number of clicks after a released button
   A negative number denotes the number of clicks while button is held down
      for a time longer than .heldDownTime milliseconds


 Copyright (C) 2010,2012, 2013 raron

  
 GNU GPLv3 license
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.



 Contact: raronzen@gmail.com

 
 History:
 2013.04.12 - FSR version
 2013.02.17 - Some improvements, simplified click codes, also made them
                 "opposite" of before (more logical in a way).
 2012.01.31 - Tiny update for Arduino 1.0 as well as retaining
                pre-Arduino 1.0 compatibility. Fixed header comments.
 2010.06.15 - First version. Basically just a small OOP
                (Object Oriented programming) excersize.
*/

#include "ClickButtonFsr.h"

ClickButtonFsr::ClickButtonFsr(uint8_t buttonPin, int lowThreshold, int highThreshold)
{
  pin = buttonPin;
  pinMode(pin, INPUT);

  // Some initial values
  active = LOW;          // Assume active-low button
  btnFlick = !active;    // initial button state in active-high logic
  lastState = btnFlick;
  click = 0;
  buttonPresses  = 0;
  depressed      = 0;
  lastBounceTime = 0;
  maxPresses     = 3;    // max button multiclick count
  debounceTime   = 20;   // Debounce timer in ms
  multiclickTime = 250;  // Time limit for multi clicks
  heldDownTime   = 1000; // time until "held-down clicks" register
  this->lowThreshold = lowThreshold;
  this->highThreshold = highThreshold;
}

int ClickButtonFsr::Update()
{
  long now = (long)millis();    // get current time
  //btnFlick = digitalRead(pin);  // current appearant button state
  fsrValue = analogRead(pin);
  if (fsrValue > highThreshold)
    btnFlick = FSR_HIGH;
  else if (fsrValue < lowThreshold)
    btnFlick = FSR_LOW;
  else
    btnFlick = FSR_MIDDLE;

  // If the switch changed, due to noise or a button press, reset the debounce timer
  if (btnFlick != lastState) lastBounceTime = now;

  // debounce the button (Check if a stable, changed state has occured)
  if (now - lastBounceTime > debounceTime)
  {
    if (!depressed && btnFlick == FSR_HIGH)
    {
      depressed = true;
      buttonPresses++;  // Button press counter
    }
    else if (depressed && btnFlick == FSR_LOW)
    {
      depressed = false;
    }
  }

  // limit number of clicks / button presses
  if (buttonPresses > maxPresses) buttonPresses = maxPresses;

  // If the button released state is stable, report nr of clicks and start new cycle
  if (!depressed && (now - lastBounceTime) > multiclickTime)
  {
    // positive count for released buttons
    click = buttonPresses;
    buttonPresses = 0;
  }

  // Check button held-down time
  if (depressed && (now - lastBounceTime > heldDownTime) && click <= maxPresses)
  {
    // negative count for held-down buttons
    click = 0 - buttonPresses;
    buttonPresses = 0;
  }

  lastState = btnFlick;
  return click;
}

