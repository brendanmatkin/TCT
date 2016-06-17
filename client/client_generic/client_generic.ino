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

#define ledPin 2


WiFiUDP udp;

const char* ssid = "TCT";
const char* password = "nosotros";
const char* deviceName = "TCT01";

IPAddress mIP(239, 0, 0, 100);    // multicast ip address
unsigned int mPort = 7777;        // multicast port

TickerScheduler ts(1);        // ts(number of task tickers)
MCP3208 adc1(15);             // adc1 on pin 15 (currently only 1 adc, but easier to add another if it's numbered)
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
  WiFi.begin(ssid, password);
  //WiFi.config(ip, gateway, subnet);
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

  ts.add(0,500,heartBeat);    // ts.add(id, period, callback, immediate fire (false)
  adc1.begin();               // init ADC (SPI)
  pinMode(ledPin, OUTPUT);    // start pin setup

  Serial.printf("WiFi connected, %s ready \r\n", deviceName);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  yield();
}


void loop() {
  ArduinoOTA.handle();
  ts.update();
  yield();

  xVal = adc1.analogRead(0);
  yVal = adc1.analogRead(1);
  sendOSCMessage("/outputModules/TCT01", xVal, yVal);
  delay(5);
}


boolean heart;
void heartBeat() {
  digitalWrite(ledPin, heart);
  heart = !heart;
}

