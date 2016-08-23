import oscP5.*;
OscP5 osc;

float xIn, yIn, zIn;
float xx, yy, rot;
float xAvg, yAvg, diam;
float lerp = 0.2;
int remoteFR;

int dips[] = new int[8];
int rssi;

void setup() {
  //size(1280, 800, FX2D);
  size(1024, 600, FX2D);

  System.setProperty("java.net.preferIPv4Stack", "true");     // disable ipv6
  osc = new OscP5(this, "239.0.0.100", 7777);               // multicast
  //osc = new OscP5(this, 9001);
  osc.plug(this, "updateIn", "/outputModules/TCT03/joystick");
  osc.plug(this, "updateDips", "/status/TCT01/dips");
  osc.plug(this, "updateRSSI", "/rssi");
  
  
  xx = width/2;
  yy = height/2;
  frameRate(60);
}

void draw() {
  //lerp = 1/(frameRate/25);
  background(10,25,70);
  noStroke();
  
  
  xAvg = xIn*lerp + (1.0-lerp)*xAvg;
  yAvg = yIn*lerp + (1.0-lerp)*yAvg;
  diam = zIn*lerp + (1.0-lerp)*diam;
  //fill(255, 255*diam);
  //ellipse(xAvg, yAvg, diam*200, diam*200);
  fill(255);
  rot+=((diam-0.5)/5);
  xx+=((xAvg-width/2)/100);
  yy+=((yAvg-height/2)/100);
  pushMatrix();
  translate(xx, yy);
  rotate(rot);
  rectMode(CENTER);
  rect(0, 0, 20, 100);
  popMatrix();
  
  
  fill(255);
  int xText = 10;
  int yText = 30;
  textSize(24);
  text("RSSI: " + rssi, xText, yText+30);
  for (int i = 0; i < 8; i++) {
    text(dips[i], xText, yText);
    xText+=15;
  }
  
  surface.setTitle("local fps: " + nf((int)frameRate,4) + " | remote fps: " + nf(remoteFR ,4)+ " | approx. receive rate: " + nf(minCount, 4) + ", " + nf(_count,4) + ", " + nf(maxCount,4) + " | Percent Loss: " + nf(percentLoss,2,1));
}


void updateDips(int... args) {
  for (int i = 0; i < args.length; i++) {
    dips[i] = args[i];
  }
  //println("DIPS!");
}


int count;
int _count;
int maxCount;
int minCount;
int FRTimer;
float percentLoss;
void updateIn(float x, float y, float z) {
  println("xIn: " + x + "  yIn: " + y);
  xIn = map(x, 0, 4096, 0, width);
  yIn = map(y, 0, 4096, height, 0);
  zIn = map(z, 0, 4096, 0, 1);
  if (millis()-FRTimer <= 1000) {
    count++;
  } else {
    _count = count;
    //percentLoss = float(remoteFR-_count)/remoteFR * 100;
    if (_count > maxCount) maxCount = _count;
    if (_count < minCount) minCount = _count;
    if (minCount == 0) minCount = _count;
    count = 0;
    FRTimer = millis();
  }
}

void updateFR(int fr) {
  remoteFR = fr; 
  //println(fr);
}

void updateRSSI(int _rssi) {
  rssi = _rssi;
  //println(rssi);
}