/* Sega Master System (SMS) Control Pad to USB
 * Auth: John Milner / jfrmilner
 * Blog Post URL: https://jfrmilner.wordpress.com/2016/07/17/arduino-project-sega-master-system-sms-control-pad-to-windowslinux-retropie-usb/
 * 
 * Date
 * July 2016
 * 
 * Hardware
 * ATmega32u4 based Arduino (Arduino Leonardo/Micro)
 * Sega Master System Control Pad - http://segaretro.org/Control_Pad_(Master_System)
 * 
 * Software
 * Arduino IDE 1.6.8
 * Arduino Joystick Library 1.0.1 - https://github.com/MHeironimus/ArduinoJoystickLibrary
 * 
 * Works with..  
 * OS (plug and play): Windows 10 / Linux RetroPie 3.8.1
 * Fusion Emulator for Sega Systems (fusion364)
 * 
 * Pinout - http://pinouts.ru/Game/sega_ms_joy_pinout.shtml
 * 
 * DE9 female plug on end view:
 * 5 4 3 2 1 
 *  9 8 7 6
 * pin 1: CONTROL_PAD_UP
 * pin 2: CONTROL_PAD_DOWN
 * pin 3: CONTROL_PAD_LEFT
 * pin 4: CONTROL_PAD_RIGHT
 * pin 5: No Connection
 * pin 6: CONTROL_PAD_B1
 * pin 7: No Connection
 * pin 8: Ground
 * pin 9: CONTROL_PAD_B2
 * 
 */
 
#include <Joystick.h>

// Arduino Controller input Pins
const int CONTROL_PAD_UP = 2;
const int CONTROL_PAD_DOWN = 3;
const int CONTROL_PAD_LEFT = 4;
const int CONTROL_PAD_RIGHT = 5;
const int CONTROL_PAD_B1 = 6;
const int CONTROL_PAD_B2 = 7;

void setup() {
  Joystick.begin(false);
  
  pinMode(CONTROL_PAD_UP, INPUT_PULLUP);  
  pinMode(CONTROL_PAD_DOWN, INPUT_PULLUP);  
  pinMode(CONTROL_PAD_LEFT, INPUT_PULLUP);  
  pinMode(CONTROL_PAD_RIGHT, INPUT_PULLUP);  
  pinMode(CONTROL_PAD_B1, INPUT_PULLUP);  
  pinMode(CONTROL_PAD_B2, INPUT_PULLUP); 

}

void loop() {
  !digitalRead(CONTROL_PAD_UP) ? Joystick.pressButton(0) : Joystick.releaseButton(0);
  !digitalRead(CONTROL_PAD_DOWN) ? Joystick.pressButton(1) : Joystick.releaseButton(1);
  !digitalRead(CONTROL_PAD_LEFT) ? Joystick.pressButton(2) : Joystick.releaseButton(2);
  !digitalRead(CONTROL_PAD_RIGHT) ? Joystick.pressButton(3) : Joystick.releaseButton(3);
  !digitalRead(CONTROL_PAD_B1) ? Joystick.pressButton(4) : Joystick.releaseButton(4);
  !digitalRead(CONTROL_PAD_B2) ? Joystick.pressButton(5) : Joystick.releaseButton(5);
  
  Joystick.sendState();
  delay(25);
}
