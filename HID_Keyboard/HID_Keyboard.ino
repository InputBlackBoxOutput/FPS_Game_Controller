/*
   File Name:Game_Controller.ino
   Description: Provides all functionality of gun controller
   Written by Rutuparn Pawar
   Created on 25 March 2020
   
   Software License Agreement:
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

///////////////////////////////////////////////////////////////////////////////////////////////
//Project includes
#include <Keyboard.h>
#include <Mouse.h>
#include "connections.h"

//Select key mapping
#include "Test.h"
//#include "Counter_Strike_1_6.h"

#define USE_MOUSE_FOR_FIRE
////////////////////////////////////////////////////////////////////////////////////////////////
//Project macros and global variables/objects

#define DEBOUNCE_WAIT  4   //ms
#define READING_DELAY  5   //ms
#define LOOP_DELAY     10  //ms 

//LINKING FUNCTIONS WITH BUTTONS
#define FIRE     B3_1
#define SCOPE    B3_2
#define RELOAD   B3_3

#define SELECT_1 B2_1
#define SELECT_2 B2_2

#define FUNC_1   B1_1
#define FUNC_2   B1_2
#define FUNC_3   B1_3
#define FUNC_4   B1_4

////////////////////////////////////////////////////////////////////////////////////////////////
void setup(void) {
  Keyboard.begin();
  Mouse.begin();

  pinMode(B1_1, INPUT_PULLUP);
  pinMode(B1_2, INPUT_PULLUP);
  pinMode(B1_3, INPUT_PULLUP);
  pinMode(B1_4, INPUT_PULLUP);

  pinMode(B2_1, INPUT_PULLUP);
  pinMode(B2_2, INPUT_PULLUP);

  pinMode(B3_1, INPUT_PULLUP);
  pinMode(B3_2, INPUT_PULLUP);
  pinMode(B3_3, INPUT_PULLUP);

  //Pin acting as ground: D8 & D4
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//Fxn to debounce and read a button
uint8_t Button_Read(uint8_t pin)
{
  delay(READING_DELAY);
  if (!digitalRead(pin))
  {
    delay(DEBOUNCE_WAIT);

    if (!digitalRead(pin))
      return 1;
    else
      return 0;
  }
  else
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
void loop(void) {
  //Key presses
  //Condition = 11
  if (digitalRead(SELECT_1) == 1 && digitalRead(SELECT_2) == 1)
  {
    if (Button_Read(FIRE))
    #ifdef USE_MOUSE_FOR_FIRE
      Mouse.click();
    #else
      Keyboard.write(FIRE_KEY);
    #endif
    
    else if (Button_Read(SCOPE))
      Keyboard.write(SCOPE_KEY);

    else if (Button_Read(RELOAD))
      Keyboard.write(RELOAD_KEY);


    if (Button_Read(FUNC_1))
      Keyboard.write(JUMP_KEY);

    else if (Button_Read(FUNC_2))
      Keyboard.write(CROUCH_KEY);

    else if (Button_Read(FUNC_3))
      Keyboard.write(GRENADE_KEY);

    else if (Button_Read(FUNC_4))
      Keyboard.write(KNIFE_KEY);

  }

  // Condition = 01
  if (digitalRead(SELECT_1) == 0 && digitalRead(SELECT_2) == 1)
  {
    if (Button_Read(FUNC_1))
      Keyboard.write(WALK_KEY);

    else if (Button_Read(FUNC_2))
      Keyboard.write(SPRINT_KEY);

    else if (Button_Read(FUNC_3))
      Keyboard.write(PREVIOUS_WEAPON_KEY);

    else if (Button_Read(FUNC_4))
      Keyboard.write(NEXT_WEAPON_KEY);
  }

  // Condition = 10
  if (digitalRead(SELECT_1) == 1 && digitalRead(SELECT_2) == 0)
  {
    if (Button_Read(FUNC_1))
      Keyboard.write(EXTRA_1_KEY);

    else if (Button_Read(FUNC_2))
      Keyboard.write(EXTRA_2_KEY);

    else if (Button_Read(FUNC_3))
      Keyboard.write(EXTRA_3_KEY);

    else if (Button_Read(FUNC_4))
      Keyboard.write(EXTRA_4_KEY);
  }

  // Condition = 00
  if (digitalRead(SELECT_1) == 0 && digitalRead(SELECT_2) == 0)
    Keyboard.write('E');

  delay(LOOP_DELAY);
}
/////////////////////////////////////////////////////////////////////////////////////////////////
