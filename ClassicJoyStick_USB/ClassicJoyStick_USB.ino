/* Commodore 64/Amiga, Atari 2600/XL/ST, ZX Spectrum, Amstrad CPC, MSX and many others compatible Joystick to USB
 * Auth: John Milner / jfrmilner
 * 
 * Version
 * 1.0
 * 
 * Date
 * May 2016
 * 
 * Hardware
 * ATmega32u4 based Arduino (Arduino Leonardo/Micro)
 * Competition Pro 5000 - http://www.vesalia.de/e_comp5000.htm (example)
 *  
 * Software
 * Arduino IDE 1.6.8
 * Arduino Joystick Library 1.0.1 - https://github.com/MHeironimus/ArduinoJoystickLibrary
 *  
 * Works with..  
 * OS (plug and play): Windows 10 / Linux RetroPie 3.8.1
 * VICE (C64 Emulator) - http://vice-emu.sourceforge.net/
 *  
 * Pinout Reference (C64) - http://old.pinouts.ru/Inputs/ControlPortC64_pinout.shtml
 * 
 * DE9 female plug on end view:
 * 5 4 3 2 1 
 *  9 8 7 6
 * pin 1: JOY_UP
 * pin 2: JOY_DOWN
 * pin 3: JOY_LEFT
 * pin 4: JOY_RIGHT
 * pin 5: No Connection
 * pin 6: JOY_BUTTON
 * pin 7: No Connection
 * pin 8: Ground
 * pin 9: No Connection
 *   
 */

#include <Joystick.h>

// Arduino Controller input Pins
const int JOY_UP = 2;
const int JOY_DOWN = 3;
const int JOY_LEFT = 4;
const int JOY_RIGHT = 5;
const int JOY_BUTTON = 6;
 
void setup() 
{
  pinMode(JOY_UP, INPUT_PULLUP);  
  pinMode(JOY_DOWN, INPUT_PULLUP);  
  pinMode(JOY_LEFT, INPUT_PULLUP);  
  pinMode(JOY_RIGHT, INPUT_PULLUP);  
  pinMode(JOY_BUTTON, INPUT_PULLUP);  

  // Initialize Joystick Library
  Joystick.begin(false);
}

void loop() 
{
  // button
  !digitalRead(JOY_BUTTON) ? Joystick.pressButton(0) : Joystick.releaseButton(0);

  // digital xy axis 
  Joystick.setXAxis(0);
  Joystick.setYAxis(0);
  if (!digitalRead(JOY_UP)) Joystick.setYAxis(-127);
  if (!digitalRead(JOY_DOWN)) Joystick.setYAxis(127);
  if (!digitalRead(JOY_LEFT)) Joystick.setXAxis(-127);
  if (!digitalRead(JOY_RIGHT)) Joystick.setXAxis(127);

  Joystick.sendState();
  
  delay(50);
}
