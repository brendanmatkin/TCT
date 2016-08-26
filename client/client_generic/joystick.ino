float pXVal, pYVal, pZVal;  // previous x, y, z

void runJoystick() {
  /* read and convert to normalized values: -1.0 to 1.0.*/
  xVal = adc1.analogRead(0);
  xVal = constrain(xVal, 0, 4096);    // not sure why this was going out of bounds, or if it even does anymore - VERIFY
  xVal = xVal / 2048 - 1;
  yVal = adc1.analogRead(1);
  yVal = constrain(yVal, 0, 4096);
  yVal = yVal / 2048 - 1;
  zVal = adc1.analogRead(2);
  zVal = constrain(zVal, 0, 4096);
  zVal = zVal / 2048 - 1;
  

  /* send only on change above xyz threshold.*/
  float xyzT = 0.05;                  // xyz send threshold
  if (abs(xVal-pXVal) > xyzT 
   || abs(yVal-pYVal) > xyzT 
   || abs(zVal-pZVal) > xyzT) {
    String  s_addr = s_moduleType;
            s_addr += "/";
            s_addr += deviceName;
            s_addr += "/joystick";    // DEVICE TYPE GOES HERE
    sendOSCMessage(s_addr.c_str(), xVal, yVal, zVal);
  }
}

