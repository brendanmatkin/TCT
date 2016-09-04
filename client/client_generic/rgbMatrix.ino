/* 
 *  TCT RGBMatrix Module (TCT06)
 *  added runRGBMatrix to schedule (1000ms)
 */

int displayIndex;
char matrixStartChar = '@'; 

void runRGBMatrix(OSCMessage &m, int addressOffset) {
  if (m.isFloat(0)) xVal = m.getFloat(0);
  if (m.isFloat(1)) yVal = m.getFloat(1);
  if (m.isFloat(2)) zVal = m.getFloat(2);

  float theta = atan2(xVal, yVal)*180/PI;
  //theta = map(theta, -PI, PI, 0, 7);
  //theta+=PI;
  if (theta < 0) theta = 360+theta;
  theta+=45;
  if (theta > 360) theta = theta-360;
  theta = map(theta, 0, 360, 0, 79);
  theta/=10;
  //Serial.println((int)theta);
  displayIndex = (int)theta;
  Serial.print(matrixStartChar);
  Serial.println(displayIndex);
}


// test method w/ no arguments
void runRGBMatrixTest() {
  if (displayIndex < 9) {
    displayIndex++;
  } else {
    displayIndex = 0;
  }
  Serial.print(matrixStartChar);
  Serial.println(displayIndex);
}

