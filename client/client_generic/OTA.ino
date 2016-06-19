void setupOTA() {
  // this is pretty much straight from the BasicOTA Example Code

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname(deviceName);

  // No authentication by default
  // ArduinoOTA.setPassword((const char *)"123");

  ArduinoOTA.onStart([]() {
    Serial.println("Start OTA");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd OTA");
    for (int i = 0; i < 3; i++) {            // number of LED blinks (3 bright)
      delay(100);
      digitalWrite(OTAledPin, LOW);          // turn LED on
      delay(100);
      digitalWrite(OTAledPin, HIGH);         // turn LED off
    }
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    digitalWrite(OTAledPin, LOW);    // LED on
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    digitalWrite(OTAledPin, HIGH);   // led off (serial call takes a few ms to run)
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
}

