float calculateRotation(float goalAngle = startAngle) {
  float difference = goalAngle - compass.heading();
  if (difference > 180) difference -= 360;
  else if (difference < -180) difference += 360;
  return difference;
}

float calculateCorrection(float goalAngle = startAngle) {

  static float derivative = 0;
  static float integrative = 0;
  static float lastValue = 0;
  static float lastMicros = micros();

  float proportional = calculateRotation(goalAngle);
  float millisElapsed = (micros() - lastMicros) / 1000.0;

  if (millisElapsed > 0.00001) {
    derivative = (proportional - lastValue) / millisElapsed;
  }
  lastMicros = micros();

  float pidCorrection = proportional * kP + derivative * kD + integrative * kI;

  integrative += proportional;

  /*Serial.println(pidCorrection);
  Bluetooth.println(pidCorrection);*/

  return pidCorrection;

}