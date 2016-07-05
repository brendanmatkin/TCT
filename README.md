# TCT
The Tangible Characterization Toolkit for Tangible Interaction Design Inc. 

# TODO

### High Priority
- [ ] Config File
- [ ] Status LEDs
- [ ] DIP switch
- [ ] TEST integration procedure w/ existing system(s)
- [ ] OSC Receive (message or bundle?)
- [ ] Send packets only on change! (don't send a new packet every cycle with no new information)
- [ ] Two modules talking without user input (or pair button)

### Lower Priority
- [ ] Determine and order batteries!
- [ ] ESP-based OTA sketch server
- [ ] SPIFFS config file
- [ ] OSC Reports
- [ ] ESP internal ADC to measure battery voltage (prevent over-discharge on LiPo w/o protection circuit)
- [ ] Write TODO List 

## Software Requirements/Wishlist (general)
* OSC for data exchange and status broadcast
* Multicast UDP (239.0.0.100:7777 or :3333)
* Only send packets when there is new information (e.g. over a certain threshold of change)
* Static IPs (for firmware updates, debug, etc)
* OTA Updates via SD card ESP Server
* m/DNS-SD (nice but not required)
* TickerScheduler Library
* JSON (file system - config files)
* WiFi.RSSI() reported on OSC status
* ADC Battery level check (and report) - Alternate: Fuel Guage IC
* Store DEVICE NAME (e.g. TCT01, TCT02, etc.) in EEPROM (easier conditionals on firmware updates?)
* Pairing mode! (hold a button to auto pair?)
* Consider timetag usage, if at all (millis since startup? Probably not necessary?)

## Hardware Requirements/Wishlist (clients)
* ESP 12E
* MCP3208 ADC (x2 each)
* MCP32017 i/o expander
* 8 DIP switch
  - Port swap
  - Behavior/Mode
  - Power source?? (USB v BAT) Just use a different charge module?
  - IP mode (Static/dynamic)
* Status LEDs (bottom/hidden):
  - Power
  - Connected
  - Firmware updating/updated
* Buttons/switches (bottom or internal):
  - Bootload button
  - Reset button
  - Request OTA update from server?
  - Power switch
  - Pairing button ?? (determine this behavior)
* LiPo (Usually 1-2x 18650 or ~1000mAh flat (e.g. 063450)

## OTA Requirements/Wishlist (Over The Air Firmware & SPIFFS (file system) update server)
* ESP 12E
* SD Card Slot
* Some buttons (not sure what is needed from this yet!)
  
## OSC Message Structure
See NetworkingInformation.xls for working details (not hosted on this repo - sensitive information). 

|Sorting Header|Device Name|Component Level|Type Tags|Arguments|
|---           |---        |---|---|---|
|/inputModules |/name      |   |   |   |
|/outputModules|/name      |   |   |   |
|/status       |/name      |   |   |   |
|/config       |/name      |   |   |   |

## Possible Modules

### Inputs
* **IR Proximity Array**
* **Accelerometer** (gravity measure? Gravity + linear)
* Finger Holes (touch, proximity, photo interrupt, etc)
* 38kHz IR (i.e. fireflys + darktorch - requires multiple modules; Zoomable darktorch?)
* Joystick
* Touch Panel (e.g. DS touch layer - no screen)
* Face tracker (Camera Vision, likely just USB + simple Procesing app to analyze & join the multicast group)
* Button(s) (like a button array to match IR Array?)
* squishy surface
* blow sensor
* 

### Outputs
Many of these would be best in multiples - or several slaves from one master.
* Baloon
* Linear Motor (fader motor or princess motor) (do this in multiples)
* **Bubble Machine**
* **Rover**
* Tethered Airplane
* String Rider?????
* Speaker cube (pregenerated set of simple beeps or honks or something)
* **Pan/Tilt head*** - generic mount (stick w/ LED, face, etc. Must be strong.)
* Antenna Motor (just build this one and make it interesting later)
* Audio Trigger (plays more complex audio e.g. binaural clips, music, PD/Max bridge)
* mini halo motion - opening flower, a bendy 'nose' (pan/tilt) -- flexible 3D print
* super tiny demo (oled probably)
* rapid inflatable + _____?
* silcone inflation dome (like pneumatible)

### Other
* OLED Display Sniffer + Message Translator (e.g. Touch OSC To devices)
  + Input buttons for mode etc.
  + Keyboard??? (assigning addresses)
  + This might be a Pi + display or something else more complex than just an ESP8266
  + DMX I/O??
