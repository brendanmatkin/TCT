import oscP5.*;
OscP5 osc;

ArrayList<Circ> circs = new ArrayList<Circ>();
int numCircs = 8;
float d = 200;

void setup() {
  //size(1280, 800, FX2D);
  size(1024, 600, FX2D);

  System.setProperty("java.net.preferIPv4Stack", "true");     // disable ipv6
  //osc = new OscP5(this, "239.0.0.100", 7777);               // multicast
  osc = new OscP5(this, 9001);
  osc.plug(this, "update0", "/domeController/0");
  osc.plug(this, "update1", "/domeController/1");
  
  float r1 = height*0.25;
  float r2 = height*0.75;
  float c = width/5;
  
  d = width/16;
  float xStart = width/2;
  float yStart = 500;
  for (int i = 0; i < 16; i+=2) {
    println(4-i/2);
    for (int j = 0; j <= (4-i/2); j++) {
      circs.add(new Circ(xStart + (d*j), yStart, d));
    }
    //circs.add(new Circ(xStart+(d*i), height/2, d));
    yStart-=d;
  }
  frameRate(60);
}

void draw() {
  background(10,25,70);
  noStroke();
  for (Circ c : circs) {
    c.display();
  }
  
  surface.setTitle((int)frameRate + " fps");
}


//void update0(int _0, int _1, int _2, int _3, int _4, int _5, int _6, int _7) {
void update0(int... args) {
  for (int i = 0; i < args.length; i++) {
    Circ c = circs.get(i);
    c.update(args[i]);
    print(nf(args[i],4,0) + ", ");
  }
}
void update1(int... args) {
  for (int i = 0; i < args.length; i++) {
    Circ c = circs.get(i+8);
    c.update(args[i]);
    print(nf(args[i],4,0) + ", ");
  }
  println();
}