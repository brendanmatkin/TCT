

/********************************************************************************************************/
// low pass filter: 
float avgA, avgB;
// send 2 ints
void sendOSCMessage(char* address, int a, int b) {
  float lerp = 0.75;
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



/********************************************************************************************************/
// send single int
void sendOSCMessage(char* address, int a) {
  OSCMessage m(address);
  if (udp.beginPacketMulticast(mIP, mPort, WiFi.localIP())){
    m.add(a).send(udp);
  }
  udp.endPacket();
  m.empty();
  yield();
}



/********************************************************************************************************/
// send 8 ints (DIP switches)
void sendOSCMessage(char* address, int _dipStates[]) {
  OSCMessage m(address);
  if (udp.beginPacketMulticast(mIP, mPort, WiFi.localIP())){
    for (int i = 0; i < 8; i++) {
      m.add(_dipStates[i]);
    }
    m.send(udp);
  }
  udp.endPacket();
  m.empty();
  yield();
}



/********************************************************************************************************/
// send 2 floats
void sendOSCMessage(char* address, float _x, float _y) {
  OSCMessage m(address);
  if (udp.beginPacketMulticast(mIP, mPort, WiFi.localIP())){
    m.add(_x).add(_y).send(udp);
  }
  udp.endPacket();
  m.empty();
  yield();
}


/********************************************************************************************************/
// does not currently support bundles
void parseOSC() {
  OSCMessage messageIn;
  int packetSize = udp.parsePacket();
  if (packetSize >0) {
    while (packetSize--) {
      messageIn.fill(udp.read());
    }
    if (!messageIn.hasError()) {
      messageIn.route("/outputModules", receiveValues);
    } else {
      Serial.println("Bad packet");
    }
  }
}



/********************************************************************************************************/
void receiveValues(OSCMessage &m, int addressOffset) {
  Serial.print("Received: /outputModules \t");
  for (int i = 0; i < m.size(); i++) {
    if (m.isInt(i)) {
      received[i] = m.getInt(i);
      Serial.print(received[i] + "\t");
    }
  }
  Serial.println();
}

