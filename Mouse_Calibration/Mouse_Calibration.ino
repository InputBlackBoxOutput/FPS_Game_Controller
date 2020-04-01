// Use this code for manually calibrating mouse movements

#include <Wire.h>
#include <Mouse.h>
#include <I2Cdev.h>
#include <MPU6050.h>

MPU6050 mpu;
int16_t ax, ay, az, gx, gy, gz;
int vx, vy;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();
  if (!mpu.testConnection()) {
    while (1);
  }
}

uint8_t movement = 0;
void loop() {
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  if (az * az + ax * ax >= 50 * 50) {
    vy = gz / 170;
    movement = 1;
  }

  if (ay * ay + ax * ax >= 50 * 50) {
    if(gy < 0)
      vx =  (gy - 100) / 300;
    if(gy > 0)
      vx =  (gy + 100) / 300;
    movement = 1;
  }

  if (movement == 1) {
    movement = 0;
    Serial.print("gz= ");
    Serial.println(gz);
    Serial.print("gy= ");
    Serial.println(gy);
    Serial.print("vx= ");
    Serial.println(vx);
    Serial.print("vy= ");
    Serial.println(vy);
    
    if ((vx >= 2 && vx <= 25) || (vx <= -2 && vx >= -25)|| (vy <= -2 && vy >= -15) || (vy >= 2 && vy <= 15))
      Mouse.move(-1 * vx, vy);
      delay(1);
   }

  delay(2);
}
