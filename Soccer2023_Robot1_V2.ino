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
int motorPins[4][3] = {{27,8,7},{24,0,1},{25,2,3},{26,4,5}};
int reverseMotor[4] = {0,1,0,1};
int speedLevel[4] = {2,0,2,0};

const float corrConst = 5;
float angle;
int speed = 25;
int maxSpeed = 30;

float calculateRotation(float goalAngle = angle);
float calculateCorrection(float goalAngle = angle);
void motorsOn();
void motorsOff();
void motorSetSpeed(int n, int speedMotor);
void go(float speedMotor, float angle, float rotation, int speedLimit = maxSpeed);

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