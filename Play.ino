void Play() {

  switch (state) {
    case PAUSED:
      
      readAll();
      motorsOff();

      if (but[2]) {
        startAngle = compass.heading();
      }

      if (but[0]) {
        startAngle = compass.heading() + 180;
        if (startAngle >= 360) {
          startAngle -= 360;
        }
      }

      if (swc[0]) state = PLAY;

      break;
    
    case PLAY:

      readAll();
      motorsOn();

      front = (lid[1] < disFront) || (lid[2] < disFront);
      left = (lid[0] < disLeft) || (lid[7] < disLeft);
      right = (lid[3] < disRight) || (lid[4] < disRight);
      back = (lid[5] < disBack) && (lid[6] < disBack);

      diffLR = int((lid[0] + lid[7]) / 2) - int((lid[3] + lid[4]) / 2);

      soccer();
      if (!swc[0]) state = PAUSED;

      break;
    
  }
}

void soccer() {

  if (seeBall) {

    if (line) {
      Line();
      return;
    }
    go(speed, ballX / ballConst);
    return;

  }

  if (back) {
    go(0,0);
    return;
  }

  directionBack = 180 + int(diffLR / backConst);
  if (directionBack > 180) directionBack -= 360;
  go(speedBack, directionBack);

}

void Line() {

  if (front) {
    go(speedLine,-180);
    return;
  } 
  
  if (left && ballX < 0) {
    go(speedLine,90);
    return;
  }
  
  if (right && ballX > 0) {
    go(speedLine,-90);
    return;
  }
  
  go(speed, ballX / ballConst);
  
}