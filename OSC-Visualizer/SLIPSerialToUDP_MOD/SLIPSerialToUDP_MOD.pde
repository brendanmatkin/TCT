import processing.serial.*;
//download at http://ubaa.net/shared/processing/udp/
import hypermedia.net.*;


/************************************************************************************
 SERIAL
 ************************************************************************************/

//the Serial communcation to the Arduino
Serial serial;

String[] serialRateStrings = {
  "300", "1200", "2400", "4800", "9600", "14400", 
  "19200", "28800", "38400", "57600", "115200"
};
int baud = 115200;
int serialListNumber = 1;

ArrayList<Byte> serialBuffer = new ArrayList<Byte>();

void setupSerial() {
  serial = new Serial(this, Serial.list()[serialListNumber], baud);
}

void stopSerial() {
  serial.stop();
}

void serialEvent(Serial serial) { 
  //decode the message
  while (serial.available () > 0) {
    slipDecode(byte(serial.read())); 
  }
}

void SerialSendToUDP() {
  byte [] buffer = new byte[serialBuffer.size()];
  //copy the buffer over
  for (int i = 0; i < serialBuffer.size(); i++) {
    buffer[i] = serialBuffer.get(i);
  }
  //send it off
  UDPSendBuffer(buffer);
  //clear the buffer
  serialBuffer.clear();
  //light up the indicator
  drawIncomingSerial();
}

void serialSend(byte[] data) {
  //encode the message and send it
  for (int i = 0; i < data.length; i++){
     slipEncode(data[i]);
  }
  //write the eot
  serial.write(eot);
}

/************************************************************************************
 SLIP ENCODING
 ************************************************************************************/

byte eot = byte(192);
byte slipesc = byte(219);
byte slipescend = byte(220);
byte slipescesc = byte(221);

byte previousByte;

void slipDecode(byte incoming) {
  byte previous = previousByte;
  previousByte = incoming;
  //if the previous was the escape char
  if (previous == slipesc) {
    //if this one is the esc eot
    if (incoming==slipescend) { 
      serialBuffer.add(eot);
    } 
    else if (incoming==slipescesc) {
      serialBuffer.add(slipesc);
    }
  } 
  else if (incoming==eot) {
    //if it's the eot
    //send off the packet
    SerialSendToUDP();
  } 
  else if (incoming != slipesc) {
    serialBuffer.add(incoming);
  }
}

void slipEncode(byte incoming) {
  if(incoming == eot){ 
    serial.write(slipesc);
    serial.write(slipescend); 
  } else if(incoming==slipesc) {  
    serial.write(slipesc);
    serial.write(slipescesc); 
  } else {
    serial.write(incoming);
  }  
}


/************************************************************************************
 UDP
 ************************************************************************************/

//UDP communication
UDP udp;

int inPort = 9000;
int outPort = 9001;
//int inPort = 7777;
//int outPort = 7777;
String ipAddress = "127.0.0.1";
//String ipAddress = "239.0.0.100";

void setupUDP() {
  udp = new UDP( this, inPort );
  //udp.log( true );     // <-- printout the connection activity
  udp.listen( true );
}

void stopUDP() {
  udp.close();
}

void UDPSendBuffer(byte[] data) {
  udp.send( data, ipAddress, outPort );
}

//called when UDP recieves some data
void receive( byte[] data) {
  drawIncomingUDP();
  //send it over to serial
  serialSend(data);
}

/************************************************************************************
 SETUP/DRAW
 ************************************************************************************/

void setup() {
  // configure the screen size and frame rate
  size(550, 250, FX2D);
  frameRate(30);
  
  setupUDP();
  setupSerial();
}

void draw() {
  background(128);
  drawIncomingPackets();
}


/************************************************************************************
 VISUALIZING INCOMING PACKETS
 ************************************************************************************/

int lastSerialPacket = 0;
int lastUDPPacket = 0;

void drawIncomingPackets() {
  //the serial packet
  fill(0);
  rect(75, 50, 100, 100);
  //the udp packet
  rect(325, 50, 100, 100);
  int now = millis();
  int lightDuration = 75;
  if (now - lastSerialPacket < lightDuration) {
    fill(255);
    rect(85, 60, 80, 80);
  }
  if (now - lastUDPPacket < lightDuration) {
    fill(255);
    rect(335, 60, 80, 80);
  }
}

void drawIncomingSerial() {
  lastSerialPacket = millis();
}

void drawIncomingUDP() {
  lastUDPPacket = millis();
}