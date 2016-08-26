/* 
 *  TCT Encoder Module (TCT05)
 *  DISABLE adc1.begin() in setup if using this! 
 */
#include <Encoder.h>
long oldPosition = 0;  // encoder library example code sets this to -999, make sure 0 is ok. 

#define ENCODER_OPTIMIZE_INTERRUPTS
Encoder encoder(12,13);              // (takes place of SPI devices - i.e. ADCs)

void runEncoder() {
  // HOLD BOOTLOAD PIN TO UPDATE OTA - interrupts from encoder mess with OTA. 
    if (digitalRead(BL_PIN) == HIGH) {
      String  s_addr = s_moduleType; 
              s_addr += "/"; 
              s_addr += deviceName; 
              s_addr += "/encoder";
              
      int encoderDifference = getEncoderDifference();
      if (encoderDifference != 0) {
        toSend[0] = encoderDifference;
        toSend[1] = oldPosition;
        sendOSCMessage(s_addr.c_str(), toSend);
        //sendOSCMessage(s_addr.c_str(), encoderDifference);  
        Serial.println(encoderDifference);
      }
    }
}

int getEncoderDifference() {
  int change = 0;
  long newPosition = encoder.read();
  if (newPosition != oldPosition) {
    change = newPosition-oldPosition;
    oldPosition = newPosition;
    //Serial.println(newPosition);
    //Serial.println(change);
  }
  return change;
}


