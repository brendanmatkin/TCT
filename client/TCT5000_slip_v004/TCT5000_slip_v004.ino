#include <OSCMessage.h>

/*
Make an OSC message and send it over serial
 */

#ifdef BOARD_HAS_USB_SERIAL
#include <SLIPEncodedUSBSerial.h>
SLIPEncodedUSBSerial SLIPSerial( thisBoardsSerialUSB );
#else
#include <SLIPEncodedSerial.h>
 SLIPEncodedSerial SLIPSerial(Serial1);
#endif


void setup() {
  //begin SLIPSerial just like Serial
  SLIPSerial.begin(115200);   // set this as high as you can reliably run on your platform
#if ARDUINO >= 100
  while(!Serial)
    ; //Leonardo "feature"
#endif
  pinMode(A0, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);
}


void loop(){
  //the message wants an OSC address as first argument
  OSCMessage msg("/sensors/TCRT5000mult");
  for (int i = 0; i < 9; i++) {
    msg.add((int32_t)analogRead(i));
  }
//  msg.add(touchRead(A8));
//  msg.add(touchRead(A9));

  SLIPSerial.beginPacket();  
    msg.send(SLIPSerial); // send the bytes to the SLIP stream
  SLIPSerial.endPacket(); // mark the end of the OSC Packet
  msg.empty(); // free space occupied by message

  delay(20);
}
