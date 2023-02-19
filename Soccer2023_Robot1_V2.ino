#include <Wire.h>
#include <IMUBoschBNO055.h>
#include <VL53L1X.h>

#define Bluetooth Serial7
#define Camera Serial8

#ifndef toRad
#define toRad(x) ((x) / 180.0 * PI)
#endif

IMUBoschBNO055 compass;
VL53L1X lidar;

int lid[8] = {0,0,0,0,0,0,0,0};
int lin[32];
bool swc[3];
bool but[4];

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}

void error(String message) {
  Serial.println(message);
  Bluetooth.println(message);
}