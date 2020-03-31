/*
   File Name:Code.ino
   Description: Provides all functionality of gun controller
   Written by Rutuparn Pawar
   Created on 25 March 2020
*/

/*
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
#include <stdint.h>
#include <Keyboard.h>
#include <Mouse.h>

#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>

//Select key mapping
#include "Testing.h"
//#include "Counter_Strike_1_6.h"
//#include "COD_Modern_Warfare_3.h"

////////////////////////////////////////////////////////////////////////////////////////////////
//Project macros and global variables/objects

#define DEBOUNCE_WAIT  4   //ms
#define READING_DELAY  5   //ms
//#define LOOP_DELAY     10  //ms 

//PIN CONNECTIONS
//TRIGGER
#define B3_1    10
//STOCK
#define B3_2    16
//MAGZINE
#define B3_3    7

//BARREL BUTTONS
#define B1_1    5
#define B1_2    6
#define B1_3    8
#define B1_4    9

//BARREL SELECT BUTTONS
#define B2_1    15
#define B2_2    14


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

MPU6050 mpu;
int16_t ax, ay, az, gx, gy, gz;
int vx, vy;
uint8_t movement = 0;

////////////////////////////////////////////////////////////////////////////////////////////////
void setup(void) {
  Keyboard.begin();
  Mouse.begin();
  
  Wire.begin();
  mpu.initialize();
  if (!mpu.testConnection()) { while (1); }

  pinMode(B1_1, INPUT_PULLUP);
  pinMode(B1_2, INPUT_PULLUP);
  pinMode(B1_3, INPUT_PULLUP);
  pinMode(B1_4, INPUT_PULLUP);

  pinMode(B2_1, INPUT_PULLUP);
  pinMode(B2_2, INPUT_PULLUP);

  pinMode(B3_1, INPUT_PULLUP);
  pinMode(B3_2, INPUT_PULLUP);
  pinMode(B3_3, INPUT_PULLUP);
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
      Keyboard.write(FIRE_KEY);

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


  //Mouse movements
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  if (az * az + ax * ax >= 65 * 65) {
    vy = gz/150;
    movement = 1;
  }

  if (ay * ay + ax * ax >= 65 * 65) {
    vx = (gy + gx/10)/150;
    movement = 1;
  }

  if (movement == 1) {
    movement = 0;
    if ((vx >= 2 && vx <= 35) || (vx <= -2 && vx >= -35)|| (vy <= -2 && vy >= -35) || (vy >= 2 && vy <= 35))
      Mouse.move(-1 * vx, vy);
      delay(1);    
  }
  
  delay(2);
}
/////////////////////////////////////////////////////////////////////////////////////////////////
