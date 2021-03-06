#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <WiFiUdp.h>

#include <OSCMessage.h>
#include <OSCBundle.h>

#include <ArduinoJson.h>
#include "FS.h"
#include <TickerScheduler.h>

#include <SPI.h>
#include <Wire.h>
#include "Adafruit_MCP23017.h"
#include <MCP3208.h>

#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

#define OTA_LED_PIN LED_PIN
#define LED_PIN 2
#define BL_PIN 0

boolean onboardLED = true;    /******** ONBOARD LED TESTING ********/

WiFiUDP udp;



/* start configurable */ 
#define fourthOctect 06
const char* deviceName = "TCT06";

const char* ssid = "TCT";
const char* password = "nosotros";

int moduleType = 0;           // 0 sender, 1 receiver, 2 sniffer, 3 converter/translator, 4+ currently null
boolean sendStatus = false;
boolean _sendDips;            // always sends when a DIP state changes.

boolean portSelect;           // TODO: implement this (DIP switch port select 7777/3333)

/* end configurable */



IPAddress statIP(10, 0, 2, fourthOctect);  // static IP  - use statIP[index] to get & set individual octets
IPAddress gateway(10, 0, 2, 254);          // gateway (for static)
IPAddress subnet(255, 255, 255, 0);        // subnet (for static)
IPAddress mIP(239, 0, 0, 100);             // multicast ip address
unsigned int mPort = 7777;                 // multicast port

TickerScheduler schedule(5);      // 0: sendOSC()
                                  // 1: heartBeatTrigger();
                                  // 2: heartBeatFade();     // not yet implemented
                                  // 3: runRGBMatrix();      // for testing
                                  // 4:

long heartBeat;                   // heartBeat timer
MCP3208 adc1(15);                 // adc1 on pin 15 (currently only 1 adc, but easier to add another if it's numbered)
Adafruit_MCP23017 io1;            // i/o expander (i2c)

/* all OSC sent or received values must be in one of these vars/arrays */
float xVal, yVal, zVal;      // OSC in/out (depending on device)
int toSend[16];              // OSC values to send
int received[16];            // OSC received values
int dipStates[8];            // dip switch states (from second register of IO)
int digitalReadValues[8];    // digital read values (from 1st register of IO)
int analogReadValues[8];     // analog read values from ADC

char* s_moduleType = "";

typedef enum {
  INIT,       // Power on and connecting to network
  HEARTBEAT,  // connected to TCT and sending OSC
  SEARCHING,  // searching for OTA update server (server not yet implemented)
  UPDATING,   // OTA UPDATE
  UPDATED,    // OTA Update successfull
  PAIRING,    // pairing mode
  PAIRED,     // pair successful
  ERR         // update failed, no network, SPIFFS mount failed, pair failed, etc.
} status_t;

status_t moduleStatus;



/* SETUP *******************************************************************************************************/
void setup() {
  Serial.begin(115200);
  Serial.println("");
  delay(1000);

  setupStatusLED();
  
  Serial.println("Mounting FS...");      // begin SPIFFS
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }

  Serial.print("Connecting to ");        // connect to WiFi
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.hostname(deviceName);             // DHCP Hostname
  WiFi.config(statIP, gateway, subnet);
  WiFi.begin(ssid, password);

  /* auto reset if it's not connecting (occasionally hangs otherwise) */
  int restartCounter = 0;
  while (!WiFi.isConnected()) {
    delay(100);
    Serial.print(".");
    restartCounter++;
    if (restartCounter > 50) ESP.restart();  // if it takes more than 5 seconds to connect, restart! 
  }
  Serial.println("  :)"); // (smiley face) yay it worked! 
  
  setupOTA();             // OTA, obviously.. see OTA.ide

  /* start listening on UDP */
  while (!udp.beginMulticast(WiFi.localIP(), mIP, mPort)) {
    delay(100);
    Serial.print("+");
  }
  Serial.println();
  yield();

  /* ticker scheduling  --  schedule.add(index, period, callback, immediate fire (false)) */
  schedule.add(0, 10, sendOSC);                
  schedule.add(1, 2000, heartBeatTrigger);
  schedule.add(2, 200, printRSSI);
  //schedule.add(3, 1000, runRGBMatrixTest);

  /* init module types: */
  switch(moduleType) {   // 0 sender, 1 receiver, 2 sniffer, 3 converter/translator, 4+ currently null
    case 0:              // sender
      //adc1.begin();      // init ADC (SPI)          // disable for encoder
      io1.begin();       // init i/o expander (i2c)
      s_moduleType = "/outputModules";
      break;
    case 1:              // receiver
      io1.begin();        // init i/o expander (i2c)
      s_moduleType = "/inputModules";
      break;
    default:
      io1.begin();        // init i/o expander (i2c)
      s_moduleType = "/misconfiguredModule";
      break;
  }
  
  /* init pins: */
  if (onboardLED) {
    pinMode(LED_PIN, OUTPUT);       // start pin setup
    digitalWrite(LED_PIN, HIGH);    // LED is active LOW
  }
  pinMode(BL_PIN, INPUT);           // has external bootstrap resistor already, no Pullup

  for (int i = 0; i < 12; i++) {  // the first 8 are the DIP, the next 4 are other inputs
    io1.pinMode(i, INPUT);
    io1.pullUp(i, HIGH);
  }
  for (int i = 12; i < 16; i++) {  // the last 4 i/o pins as outputs.
    io1.pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }

  /* who am I this time?  */
  Serial.println("\r\nWiFi connected.");
  Serial.printf("%s (%s) ready. \r\n", deviceName, WiFi.macAddress().c_str());
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
//  String destIP; char *dest;
//  if (multicast) {
//    destIP = mIP.toString() + ":" + mPort;
//    dest = "MULTICAST";
//  }
//  else {
//    destIP = rIP.toString() + ":" + uSendPort;
//    dest = "UNICAST";
//  }
//  Serial.printf("Sending %s OSC to: ", dest);
//  Serial.println(destIP);
//
//  // get file version
//  String sketchName = __FILE__;
//  int snLength = sketchName.length();
//  char vA = sketchName.charAt(snLength-7);
//  char vB = sketchName.charAt(snLength-6);
//  char vC = sketchName.charAt(snLength-5);
//  Serial.printf("Firmware: v. %c%c%c \r\n\r\n", vA, vB, vC);

  String s_addr = s_moduleType; s_addr += "/"; s_addr += deviceName; s_addr += "/joystick";
  Serial.printf("osc address: %s \r\n", s_addr.c_str());
  
  yield();  // let the wifi stack do its thing
}




/* LOOP *******************************************************************************************************/
void loop() {
  ArduinoOTA.handle();
  schedule.update();
  if (heartBeat - millis() > 1) {
    if (onboardLED) digitalWrite(LED_PIN, HIGH);  // turn off the led
  }
  yield();

  parseOSC();
  
  /* check DIP switches */
  _sendDips = false;
  for (int i = 0; i < 8; i++) {
    uint8_t temp = io1.digitalRead(i);
    if (temp != dipStates[i]) {
      _sendDips = true;
      dipStates[i] = temp;
    }
  }
  
  //sendOSC(); // currently controlled by the scheduler

  updateStatusLED();

  //Serial.println(frameRate());
  //delay(5);
  yield();
}





/********************************************************************************************************/
void heartBeatTrigger() {
  heartBeat = millis();
  if (onboardLED) digitalWrite(LED_PIN, LOW);
}

void printRSSI() {
  //Serial.println(WiFi.RSSI());
  sendOSCMessage("/rssi", WiFi.RSSI());
}



/********************************************************************************************************/
// frameRate (not super important but interesting for early development & tests
unsigned int prevTime;
float avgFR;
float frLerp = 0.01;
int frameRate() {
  unsigned int t = millis()-prevTime;
  float fr = 1000/(float)t;
  avgFR = frLerp*fr + (1.0-frLerp)*avgFR;
  prevTime = millis();
  return (int)avgFR;
}
