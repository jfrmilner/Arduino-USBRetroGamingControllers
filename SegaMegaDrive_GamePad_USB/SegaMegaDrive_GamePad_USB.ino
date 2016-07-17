/* Sega MegaDrive/Genesis controller to USB
 * 
 * Version: 1.1 Date: 9/29/2014 Author: Jon Thysell <thysell@gmail.com>
 * Reads buttons presses from Sega Genesis 3/6 button controllers
 * and reports their state via keyboard button presses. Handles hot
 * swapping of controllers and auto-switches between 3 and 6 button
 * polling patterns.
 * Blog Post URL: https://jonthysell.com/2014/09/29/sega-genesis-controllers-and-arduino-revisited/
 * 
 * Version: 1.1.1 Date: July/2016 Author: John Milner / jfrmilner
 * Modified to use https://github.com/MHeironimus/ArduinoJoystickLibrary instead of keyboard.
 * Blog Post URL: https://jfrmilner.wordpress.com/2016/07/17/arduino-project-sega-megadrivegenesis-controller-to-windowslinux-retropie-usb/
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
 * Fusion Emulator for Sega Systems (fusion364)
 * 
 * Pinout Reference - http://pinouts.ru/Game/genesiscontroller_pinout.shtml
 * 
 * DE9 female plug on end view (Player0):
 * 5 4 3 2 1 
 *  9 8 7 6
 * pin 1: UP/Z
 * pin 2: DOWN/Y
 * pin 3: LEFT/X
 * pin 4: RIGHT/MODE
 * pin 5: +5v
 * pin 6: A/B
 * pin 7: SELECT[]
 * pin 8: Ground
 * pin 9: START/C
 * 
 */
 
#include <Joystick.h>
const int PLAYERS = 2;
 
// Controller Button Flags
const int ON = 1;
const int UP = 2;
const int DOWN = 4;
const int LEFT = 8;
const int RIGHT = 16;
const int START = 32;
const int A = 64;
const int B = 128;
const int C = 256;
const int X = 512;
const int Y = 1024;
const int Z = 2048;
const int MODE = 4096;
 
// Controller DB9 Pin 7 Mappings. Controller 0,1
const int SELECT[] = { 8, 9 };
 
typedef struct
{
  int player;
  int pin;
  int lowFlag;
  int highFlag;
  int pulse3Flag;
} input;
 
// Controller DB9 Pin to Button Flag Mappings
// First column is the controller index, second column
// is the Arduino pin that the controller's DB9 pin is
// attached to
input inputMap[] = {
  { 0,  2,  UP,    UP,     Z    }, // P0 DB9 Pin 1
  { 0,  3,  DOWN,  DOWN,   Y    }, // P0 DB9 Pin 2
  { 0,  4,  ON,    LEFT,   X    }, // P0 DB9 Pin 3
  { 0,  5,  ON,    RIGHT,  MODE }, // P0 DB9 Pin 4
  { 0,  6,  A,     B,      0    }, // P0 DB9 Pin 6
  { 0,  7,  START, C,      0    }, // P0 DB9 Pin 9
  { 1,  A0, UP,    UP,     Z    }, // P1 DB9 Pin 1
  { 1,  A1, DOWN,  DOWN,   Y    }, // P1 DB9 Pin 2
  { 1,  A2, ON,    LEFT,   X    }, // P1 DB9 Pin 3
  { 1,  A3, ON,    RIGHT,  MODE }, // P1 DB9 Pin 4
  { 1,  A4, A,     B,      0    }, // P1 DB9 Pin 6
  { 1,  A5, START, C,      0    }  // P1 DB9 Pin 9
};
 
typedef struct
{
  int player;
  int flag;
  char key;
} output;
 
// Controller Button Flag to Keyboard Mappings
// First column is the controller index, second column
// is the button flag, third is joystick button
output outputMap[] = {
  { 0, UP,    0 },
  { 0, DOWN,  1 },
  { 0, LEFT,  2 },
  { 0, RIGHT, 3 },
  { 0, START, 4 },
  { 0, A,     5 },
  { 0, B,     6 },
  { 0, C,     7 },
  { 0, X,     8 },
  { 0, Y,     9 },
  { 0, Z,     10 },
  { 0, MODE,  11 },
  { 1, UP,    12 },
  { 1, DOWN,  13 },
  { 1, LEFT,  14 },
  { 1, RIGHT, 15 },
  { 1, START, 16 },
  { 1, A,     17 },
  { 1, B,     18 },
  { 1, C,     19 },
  { 1, X,     20 },
  { 1, Y,     21 },
  { 1, Z,     22 },
  { 1, MODE,  23 }
};

 
// Controller State
int currentState[] = { 0, 0 };
int lastState[] = { -1, -1 };
 
// Default to three-button mode until six-button connects
boolean sixButtonMode[] = { false, false };
 
void setup()
{
  // Setup input pins
  for (int i = 0; i < sizeof(inputMap) / sizeof(input); i++)
  {
    pinMode(inputMap[i].pin, INPUT);
    digitalWrite(inputMap[i].pin, HIGH);
  }
  
  // Setup select pins
  for (int i = 0; i < PLAYERS; i++)
  {
    pinMode(SELECT[i], OUTPUT);
    digitalWrite(SELECT[i], HIGH);
  }

  // Initialize Joystick Library
  Joystick.begin(false);  
}
 
void loop()
{
  readButtons();
  sendStates();
}
 
void readButtons()
{
  for (int i = 0; i < PLAYERS; i++)
  {
    resetState(i);
    if (sixButtonMode[i])
    {
      read6buttons(i);
    }
    else
    {
      read3buttons(i);
    }
  }
}
 
void resetState(int player)
{
  currentState[player] = 0;
}
 
void read3buttons(int player)
{
  // Set SELECT LOW and read lowFlag
  digitalWrite(SELECT[player], LOW);
     
  delayMicroseconds(20);
     
  for (int i = 0; i < sizeof(inputMap) / sizeof(input); i++)
  {
    if (inputMap[i].player == player && digitalRead(inputMap[i].pin) == LOW)
    {
      currentState[player] |= inputMap[i].lowFlag;
    }
  }
 
  // Set SELECT HIGH and read highFlag
  digitalWrite(SELECT[player], HIGH);
     
  delayMicroseconds(20);
     
  for (int i = 0; i < sizeof(inputMap) / sizeof(input); i++)
  {
    if (inputMap[i].player == player && digitalRead(inputMap[i].pin) == LOW)
    {
      currentState[player] |= inputMap[i].highFlag;
    }
  }
    
  // When a six-button first connects, it'll spam UP and DOWN,
  // which signals the game to switch to 6-button polling
  if (currentState[player] == (ON | UP | DOWN))
  {
    sixButtonMode[player] = true;
  }
  // When a controller disconnects, revert to three-button polling
  else if ((currentState[player] & ON) == 0)
  {
    sixButtonMode[player] = false;
  }
  
  delayMicroseconds(20);
}
 
void read6buttons(int player)
{
  // Poll for three-button states twice
  read3buttons(player);
  read3buttons(player);
  
  // After two three-button polls, pulse the SELECT line
  // so the six-button reports the higher button states
  digitalWrite(SELECT[player], LOW);
  delayMicroseconds(20);
  digitalWrite(SELECT[player], HIGH);
  
  for(int i = 0; i < sizeof(inputMap) / sizeof(input); i++)
  {
    if (inputMap[i].player == player && digitalRead(inputMap[i].pin) == LOW)
    {
      currentState[player] |= inputMap[i].pulse3Flag;
    }
  }
  
  delayMicroseconds(1360); // Increased from 1000 for Sega Mega Drive original Arcade 6 Button Pad Controller MK-1653-50
}
 
void sendStates()
{
  for (int i = 0; i < sizeof(outputMap) / sizeof(output); i++)
  {
    int last = (lastState[outputMap[i].player] & outputMap[i].flag);
    int current = (currentState[outputMap[i].player] & outputMap[i].flag);
     
    if (last != current)
    {
      if (current == outputMap[i].flag)
      {
        Joystick.pressButton(outputMap[i].key);
      }
      else
      {
        Joystick.releaseButton(outputMap[i].key);
      }
    }
  }
  
  for (int i = 0; i < PLAYERS; i++)
  {
    lastState[i] = currentState[i];
  }
  Joystick.sendState();
}
