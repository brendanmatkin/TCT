// low pass filter: 
float avgA, avgB;

void sendOSCMessage(char* address, int a, int b) {
  float lerp = 0.2;
  avgA = lerp*a + (1.0-lerp)*avgA;
  avgB = lerp*b + (1.0-lerp)*avgB;
  
  OSCMessage m(address);
  if (udp.beginPacketMulticast(mIP, mPort, WiFi.localIP())){
    m.add((int)avgA).add((int)avgB).send(udp);
  }
  udp.endPacket();
  m.empty();
  yield();
}

