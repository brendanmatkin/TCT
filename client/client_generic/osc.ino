

/********************************************************************************************************/
void sendOSC() {   // currently triggered by the scheduler
  //digitalWrite(LED_PIN, LOW);  // onboard LED on

  
  if (moduleType == 0) {
    //runEncoder();
    //runJoystick();
  }

  // send status, DIP switches, debug.
  String  s_addr = "/status/";
          s_addr += deviceName;
  String  s_addr2 = s_addr + "/dips";
  
  if (sendStatus && _sendDips) {
    sendOSCMessage(s_addr2.c_str(), dipStates);
  }
          s_addr2 = s_addr + "/frameRate";
  if (sendStatus) {
    sendOSCMessage(s_addr2.c_str(), frameRate());
  }
  
  //digitalWrite(LED_PIN, HIGH);  // onboard LED off
}



/********************************************************************************************************/
// send single int
void sendOSCMessage(const char* address, int a) {
  OSCMessage m(address);
  if (udp.beginPacketMulticast(mIP, mPort, WiFi.localIP())){
    m.add(a).send(udp);
  }
  udp.endPacket();
  m.empty();
  yield();
}



/********************************************************************************************************/
// send 2 ints 
float avgA, avgB;  // low pass filter
void sendOSCMessage(const char* address, int a, int b) {
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
// send 8 ints via an array (DIP switches)
void sendOSCMessage(const char* address, int _dipStates[]) {
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
void sendOSCMessage(const char* address, float _x, float _y) {
  OSCMessage m(address);
  if (udp.beginPacketMulticast(mIP, mPort, WiFi.localIP())){
    m.add(_x).add(_y).send(udp);
  }
  udp.endPacket();
  m.empty();
  yield();
}

// send 3 floats
void sendOSCMessage(const char* address, float _x, float _y, float _z) {
  OSCMessage m(address);
  if (udp.beginPacketMulticast(mIP, mPort, WiFi.localIP())){
    m.add(_x).add(_y).add(_z).send(udp);
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
      //if (messageIn.route("/outputModules", receiveValues));   // generic receive
      if (!messageIn.route("/outputModules", runRGBMatrix)){     // RGBMatrix receive
        //Serial.println("Unrouted Message");     
      }
    } else {
      Serial.println("Received bad packet");
    }
  }
}



/********************************************************************************************************/
void receiveValues(OSCMessage &m, int addressOffset) {
  //Serial.print("Received: /outputModules \t");
  for (int i = 0; i < m.size(); i++) {
    if (m.isInt(i)) {
      received[i] = m.getInt(i);
      Serial.print(received[i] + "\t");
    }
  }
  Serial.println("endPacket");
}

