void sendOSCMessage(String address, int a, int b) {
  OSCMessage m(address);
  if (udp.beginPacketMulticast(mIP, mPort, WiFi.localIP())){
    m.add(a).add(b).send(udp);
  }
  udp.endPacket();
  m.empty();
  yield();
}

