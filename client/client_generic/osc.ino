// low pass filter: 
float avgA, avgB;
float lerp = 0.75;

void sendOSCMessage(char* address, int a, int b) {
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

void sendOSCMessage(char* address, int a) {
  OSCMessage m(address);
  if (udp.beginPacketMulticast(mIP, mPort, WiFi.localIP())){
    m.add(a).send(udp);
  }
  udp.endPacket();
  m.empty();
  yield();
}
