/* Super Nintendo Entertainment System (SNES) Controller/Gamepad to USB
 * Auth: John Milner / jfrmilner
 * Blog Post URL: https://jfrmilner.wordpress.com/2016/07/17/arduino-project-super-nintendo-entertainment-system-snes-controllergamepad-to-windowslinux-retropie-usb/
 * 
 * Version
 * 1.0
 * 
 * Date
 * July 2016
 * 
 * Hardware
 * ATmega32u4 based Arduino (Arduino Leonardo/Micro)
 * Super Nintendo Entertainment System Controller
 *  
 * Software
 * Arduino IDE 1.6.8
 * Arduino Joystick Library 1.0.1 - https://github.com/MHeironimus/ArduinoJoystickLibrary
 *  
 * Works with..  
 * OS (plug and play): Windows 10 / Linux RetroPie 3.8.1
 * Snes9x (SNES Emulator) - http://www.snes9x.com/ / https://tcrf.net/SNES_Test_Program
 * 
 * Pinout - http://pinoutsguide.com/Game/snescontroller_pinout.shtml
 * 
 * 7 pin SNES proprietary female connector view:
 *  -----------------\
 * | 1 2 3 4 | 5 6 7 | 
 *  -----------------/
 * pin 1: +5v
 * pin 2: Data Clock
 * pin 3: Data Latch
 * pin 4: Serial Data
 * pin 5: No Connection
 * pin 6: No Connection
 * pin 7: Ground
 * 
 */

#include <Joystick.h>

// Controller Buttons
#define SNES_B       1      //000000000001
#define SNES_Y       2      //000000000010
#define SNES_SELECT  4      //000000000100
#define SNES_START   8      //000000001000
#define SNES_UP      16     //000000010000
#define SNES_DOWN    32     //000000100000
#define SNES_LEFT    64     //000001000000
#define SNES_RIGHT   128    //000010000000
#define SNES_A       256    //000100000000
#define SNES_X       512    //001000000000
#define SNES_L       1024   //010000000000
#define SNES_R       2048   //100000000000

const int PIN_CLOCK = 6;
const int PIN_LATCH = 7;
const int PIN_DATA = 12;

void setup(){
  Joystick.begin(false);
  
  pinMode(PIN_CLOCK, OUTPUT);
  digitalWrite(PIN_CLOCK, HIGH);
  pinMode(PIN_LATCH, OUTPUT);
  digitalWrite(PIN_LATCH, LOW);
  pinMode(PIN_DATA, INPUT_PULLUP);
}

void loop(){
  uint16_t state = 0;
    // 12us latch
    digitalWrite(PIN_LATCH, HIGH);
    delayMicroseconds(12);
    digitalWrite(PIN_LATCH, LOW);
    delayMicroseconds(6);
    // Retrieve 4021s sixteen bits of data
    for(int i = 0; i < 16; i++){
        digitalWrite(PIN_CLOCK, LOW);
        delayMicroseconds(6);
        state |= digitalRead(PIN_DATA) << i;
        digitalWrite(PIN_CLOCK, HIGH);
        delayMicroseconds(6);
    }

  // buttons
  SNES_B & ~state ? Joystick.pressButton(0) : Joystick.releaseButton(0);
  SNES_Y & ~state ? Joystick.pressButton(1) : Joystick.releaseButton(1);
  SNES_SELECT & ~state ? Joystick.pressButton(2) : Joystick.releaseButton(2);
  SNES_START & ~state ? Joystick.pressButton(3) : Joystick.releaseButton(3);
  SNES_A & ~state ? Joystick.pressButton(4) : Joystick.releaseButton(4);
  SNES_X & ~state ? Joystick.pressButton(5) : Joystick.releaseButton(5);
  SNES_L & ~state ? Joystick.pressButton(6) : Joystick.releaseButton(6);
  SNES_R & ~state ? Joystick.pressButton(7) : Joystick.releaseButton(7);

  // 360° Hat Switch 0
  Joystick.setHatSwitch(0, -1); // release
  if (SNES_UP & ~state) Joystick.setHatSwitch(0, 0);
  if (SNES_RIGHT & ~state) Joystick.setHatSwitch(0, 90);
  if (SNES_DOWN & ~state) Joystick.setHatSwitch(0, 180);
  if (SNES_LEFT & ~state) Joystick.setHatSwitch(0, 270);
  if (SNES_UP & ~state && SNES_RIGHT & ~state) Joystick.setHatSwitch(0, 45);
  if (SNES_DOWN & ~state && SNES_RIGHT & ~state) Joystick.setHatSwitch(0, 135);
  if (SNES_DOWN & ~state && SNES_LEFT & ~state) Joystick.setHatSwitch(0, 225);
  if (SNES_UP & ~state && SNES_LEFT & ~state) Joystick.setHatSwitch(0, 315);
  
  Joystick.sendState();
  delay(25);
}

