float calculateRotation(float goalAngle = angle) {
  float difference = compass.heading() - goalAngle;
  if (difference > 180) difference -= 360;
  else if (difference < -180) difference += 360;
  return difference;
}

float calculateCorrection(float goalAngle = angle) {
  return calculateRotation(goalAngle) / corrConst;
}