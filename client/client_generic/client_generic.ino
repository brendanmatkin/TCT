#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <ArduinoJson.h>
#include "FS.h"
#include <TickerScheduler.h>
#include <MCP3208.h>
#include <SPI.h>

#define OTA_LED_PIN LED_PIN
#define LED_PIN 2

WiFiUDP udp;

/* start configurable */ 
#define fourthOctect 01
const char* deviceName = "TCT01";

const char* ssid = "TCT";
const char* password = "nosotros";

boolean sendStatus = false;
/* end configurable*/

IPAddress statIP(10, 0, 2, fourthOctect);  // static IP  - use statIP[index] to get & set individual octets
IPAddress gateway(10, 0, 2, 254);          // gateway (for static)
IPAddress subnet(255, 255, 255, 0);        // subnet (for static)
IPAddress mIP(239, 0, 0, 100);             // multicast ip address
unsigned int mPort = 7777;                 // multicast port

TickerScheduler schedule(5);      // schedule(number of task tickers)
long heartBeat;                   // heartBeat timer
MCP3208 adc1(15);                 // adc1 on pin 15 (currently only 1 adc, but easier to add another if it's numbered)
int xVal, yVal;


void setup() {
  Serial.begin(115200);
  Serial.println("");
  delay(1000);
  
  Serial.println("Mounting FS...");      // begin SPIFFS
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }

  Serial.print("Connecting to ");        // connect to WiFi
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.config(statIP, gateway, subnet);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    delay(3000);
    ESP.restart();
  }

  setupOTA();     // OTA
  
  while (!udp.beginMulticast(WiFi.localIP(), mIP, mPort)) {   // start UDP
    delay(100);
    Serial.print("+");
  }
  Serial.println();
  yield();

  schedule.add(0,2000,heartBeatTrigger);    // schedule.add(id, period, callback, immediate fire (false)
  adc1.begin();               // init ADC (SPI)
  pinMode(LED_PIN, OUTPUT);    // start pin setup
  digitalWrite(LED_PIN, HIGH); // LED is active LOW

  Serial.printf("WiFi connected, %s ready \r\n", deviceName);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  yield();
}


void loop() {
  ArduinoOTA.handle();
  schedule.update();
  if (heartBeat - millis() > 1) {
    digitalWrite(LED_PIN, HIGH);  // turn off the led
  }
  yield();

  
  int _xVal = adc1.analogRead(0);
  int _yVal = adc1.analogRead(1);

  /* only send message after crossing a change threshold
   * TODO improve this algorithm - consider adding a flag + counter
   * to track whether there is no change for multiple cycles.
   */
  int thresh = 2;
  if (abs(xVal-_xVal) > thresh || abs(yVal-_yVal) > thresh) {
    sendOSCMessage("/outputModules/TCT01", _xVal, _yVal);
  }
  xVal = _xVal;
  yVal = _yVal;
  
  
  if (sendStatus) sendOSCMessage("/status/TCT01/frameRate", frameRate());

  //Serial.println(frameRate());
  delay(5);
  //yield();
}



void heartBeatTrigger() {
  heartBeat = millis();
  digitalWrite(LED_PIN, LOW);
}



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

