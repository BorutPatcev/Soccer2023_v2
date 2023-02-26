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

#define kP 2.0
#define kD 0.5
#define kI 0.1

int lid[8] = {0,0,0,0,0,0,0,0};
int lin[32];
bool swc[3];
bool but[4];

int motorPins[4][3] = {{27,8,7},{24,0,1},{25,2,3},{26,4,5}};
int reverseMotor[4] = {1,1,0,0};
int speedLevel[4] = {0,5,0,3};

const float corrConst = 2;
const float ballConst = 2.1;
const float backConst = 15;
float angle;

int speed = 40;
int maxSpeed = 45;
int speedBack = 30;
int speedLine = 20;

int ballX = 0;
int ballY = 0;
int treshold = 300;
int camShiftX = 15;

int disFront = 500;
int disLeft = 400;
int disRight = 400;
int disBack = 400;
int diffLR = 0;
int centerLimit = 200;
int directionBack = 180;
int disGoalkeeper = 55;
int disBackClose = 350;
int ballGoalkeeper = 20;

bool line = false;
bool seeBall = false;
bool front;
bool left;
bool right;
bool back;
bool backClose;

unsigned long timerLidar = 0;

float calculateRotation(float goalAngle = angle);
float calculateCorrection(float goalAngle = angle);

void motorsOn();
void motorsOff();
void motorSetSpeed(int n, int speedMotor);
void go(float speedMotor, float angle, float rotation = calculateCorrection(), int speedLimit = maxSpeed);

void selectLidar(uint8_t i);

void setupMotors();
void setupLineSensors();
void setupButtonsSwitches();
void setupLidars();
void Setup();

void readAll();
void readLineSensors();
void readLidars();
void readSwitches();
void readButtons();
void readCamera();

void setup() {
  
  Setup();

}

void loop() {
  
  readAll();

  if (but[2]) {
    angle = compass.heading();
  }

  if (but[0]) {
    angle = compass.heading() + 180;
    if (angle >= 360) {
      angle -= 360;
    }
  }

  while (swc[0]) {
    
    readAll();
    motorsOn();

    front = (lid[1] < disFront) || (lid[2] < disFront);
    left = (lid[0] < disLeft) || (lid[7] < disLeft);
    right = (lid[3] < disRight) || (lid[4] < disRight);
    back = (lid[5] < disBack) && (lid[6] < disBack);

    diffLR = int((lid[0] + lid[7]) / 2) - int((lid[3] + lid[4]) / 2);

    if (seeBall && ballY > disGoalkeeper) {
      if (line) {
        if (front) {
          go(speedLine,-180,calculateCorrection());
        } else if (left && ballX < 0) {
          go(speedLine,90,calculateCorrection());
        } else if (right && ballX > 0) {
          go(speedLine,-90,calculateCorrection());
        } else {
          go(speed, ballX / ballConst, calculateCorrection());
        }
      } else {
        go(speed, ballX / ballConst, calculateCorrection());
      }
    } else {
      if (back) {
        go(0,0,calculateCorrection());
      } else {
        directionBack = 180 + int(diffLR / backConst);
        if (directionBack > 180) directionBack -= 360;
        go(speedBack, directionBack, calculateCorrection());
      }
    }
  }
  
  motorsOff();

}

void selectLidar(uint8_t i) {
  
  if (i > 7) return;
 
  Wire.beginTransmission(0x70);
  Wire.write(1 << i);
  Wire.endTransmission();
  
}

void error(String message) {
  Serial.println(message);
  Bluetooth.println(message);
}