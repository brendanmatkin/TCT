
#include "Arduino.h"
#include "TCT.h"

// probably do the OSC in here!  --  does this mean I need UDP in here too? 
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <WiFiUdp.h>

TCT::TCT(module_type_t moduleType) {
	_moduleType = moduleType;
}

void TCT::sendData() {
	// send sensor data over OSC
}

void TCT::sendInfo() {
	// send device info over OSC
}

void TCT::checkMode() {
	// check DIP switches 
}

bool TCT::pairMode(bool buttonPress, uint8_t timeout) {
	return paired;
}

void TCT::statusLED(status_led_t sL) {

}

