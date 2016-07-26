#ifndef _TCT_h_
#define _TCT_h_

#include "Arduino.h"
#include <WiFiUDP.h>



typedef enum {
  INPUT_MODULE
  OUTPUT_MODULE
  SNIFFER_MODULE
  TRANSLATOR_MODULE
} module_type_t;

typedef enum {
  CONNECTED
  UPDATING
  UPDATE_SUCCESSFUL
  UPDATE_FAILED
} status_led_t;

class TCT {
  public: 
    TCT(module_type_t mT);

    void sendData(float _a, float _b);  // send input data
    void sendInfo();                    // send module information: name, battery voltage, module type, mode, FW version, etc.
    void checkMode();  					        // check DIP switches
    void readIncoming();				        // parse incoming OSC messages (maybe return something?)

    bool pairMode(bool buttonPress, uint8_t timeout);   	// returns true when paired

    void statusLED(status_led_t sL);  	// make the light do the things!
    void requestOTA();        					// request OTA update from server (/check for new version)

    uint8_t beginMulticast(IPAddress _interface, IPAddress _multi, uint16_t _port);		// pass through to WiFiUDP

  private:
  	module_type_t _moduleType;
  	void checkConfig();

    uint8_t _timeout;
    bool _paired;

  	virtual int beginPacketMulticast(IPAddress _multi, uint16_t _port, IPAddress _interface, int ttl = 1);  // pass WiFiUDP
  	virtual int endPacket();			// pass WiFiUDP
};

#endif 
/* _TCT_h_ */