void readAll() {

  readLineSensors();
  readLidars();
  readSwitches();
  readButtons();
  
}

void readLineSensors() {

  int counter = 0;
  
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      for (int k = 0; k < 2; k++) {
        for (int l = 0; l < 2; l++) {
          
          digitalWrite(32,i);
          digitalWrite(33,j);
          digitalWrite(30,k);
          digitalWrite(31,l);

          delay(1);

          lin[counter] = analogRead(A16);
          lin[counter + 16] = analogRead(A17);

          counter++;
          
        }
      }
    }
  }

}

void readLidars() {

  for (int i = 0; i < 8; i++) {
    selectLidar(i);
    lid[i] = lidar.read();
  }
  
}

void readSwitches() {
  
  swc[0] = digitalRead(23);
  swc[1] = digitalRead(22);
  swc[2] = digitalRead(17);
  
}

void readButtons() {
  but[0] = digitalRead(16);
  but[1] = digitalRead(39);
  but[2] = digitalRead(38);
  but[3] = digitalRead(37);
  
}