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
int reverseMotor[4] = {1,1,0,0};
int speedLevel[4] = {0,5,0,3};

const float corrConst = 1.5;
float angle;

int speed = 35;
int maxSpeed = 40;

int ballX = 0;
int ballY = 0;
int treshold = 300;
int camShiftX = 15;
bool line = false;
bool seeBall = false;

unsigned long timerLidar = 0;

float calculateRotation(float goalAngle = angle);
float calculateCorrection(float goalAngle = angle);

void motorsOn();
void motorsOff();
void motorSetSpeed(int n, int speedMotor);
void go(float speedMotor, float angle, float rotation, int speedLimit = maxSpeed);

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

  while (swc[0]) {
    
    int lidgol;
    
    readAll();
    motorsOn();
    lidgol=lid[0]-lid[3];
    if (ballX == -160 && lid[5]>500 && lid[6]>500 ) {
      if(lidgol>150){
        go(speed,-155,calculateCorrection());
      }
      else if(lidgol<-150){
        go(speed,155,calculateCorrection());
      }
      else{
      go(speed,-180,calculateCorrection());
      }
    }
    else if(ballX == -160 && lid[5]<500 && lid[6]<500){
      go(0,0,calculateCorrection());
    } else {
      if (line && ballX != -160) {
        //Bluetooth.println(ballX - 160);
        if ((ballX) >= 40 && lid[3] < 300 && lid[4] < 300) {
          go(0,0,calculateCorrection());
        } else if ((ballX) <= -40 && lid[0] < 300 && lid[7] < 300) {
          go(0,0,calculateCorrection());
        } else if (((ballX) > 40 && (ballX) < -40) || lid[1] < 500 && lid[2] < 500) {
          go(0,0,calculateCorrection());
        } else {
          go(speed - int(abs((ballX)/10)),(ballX) / 2, calculateCorrection());
        }
      } else {
        go(speed - int(abs((ballX)/10)),(ballX) / 2, calculateCorrection());
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