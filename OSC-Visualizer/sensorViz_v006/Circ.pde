class Circ {
  float xPos, yPos;
  int alpha, diam;
  int maxDiam;
  float newAlpha, newDiam;
  
  int min = 80;    // 0
  int max = 1000; // 1024
  
  Circ(float xPos, float yPos, float maxDiam) {
    this.xPos = xPos;
    this.yPos = yPos;
    this.maxDiam = (int)maxDiam;
    
    // temp stuff below here
    diam = (int)maxDiam;
    newDiam = diam;
    alpha = 255;
    newAlpha = alpha;
  }
  
  void update(int val) {
    newAlpha = map(val, min, max, 255, 0);
    newDiam = map(val, min, max, maxDiam, 0);
  }
  
  void display() {
    float lerp = 0.3; // was 0.1                        // how quickly to average (0-1)
    diam = int(lerp*newDiam + (1.0-lerp)*diam);         // this is more correct: the final value should be previous reading and first value should be new reading.
    alpha = int(lerp*newAlpha + (1.0-lerp)*alpha);

    fill(255,alpha);
    ellipse(xPos, yPos, diam, diam);
  }
}