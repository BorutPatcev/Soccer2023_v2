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

enum State {
  PAUSED,
  PLAY
};

State state = PAUSED;

const float ballConst = 2.1;
const float backConst = 15;
float startAngle;

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

float calculateRotation(float goalAngle = startAngle);
float calculateCorrection(float goalAngle = startAngle);

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
void Play();

void readAll();
void readLineSensors();
void readLidars();
void readSwitches();
void readButtons();
void readCamera();

void soccer();
void Line();

void setup() {
  
  Setup();

}

void loop() {
  
  Play();

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