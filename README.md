# TCT
The Tangible Characterization Toolkit for Tangible Interaction Design Inc. 

# TODO

### High Priority
- [ ] OSC Receive (message or bundle?)
- [ ] SPIFFS config file
- [ ] Status LEDs

### Low Priority
- [ ] OSC Reports
- [ ] Write TODO List

## Software Requirements/Wishlist (general)
* OSC for data exchange and status broadcast
* Multicast UDP (239.0.0.100:7777 or :3333)
* Static IPs (for firmware updates, debug, etc)
* OTA Updates via SD card ESP Server
* m/DNS-SD (nice but not required)
* TickerScheduler Library
* JSON (file system - config files)
* WiFi.RSSI() reported on OSC status
* ADC Battery level check (and report) - Alternate: Fuel Guage IC
* Store DEVICE NAME (e.g. TCT01, TCT02, etc.) in EEPROM (easier conditionals on firmware updates?)
* Pairing mode! (hold a button to auto pair?)

## Hardware Requirements/Wishlist (clients)
* ESP 12E
* MCP3208 ADC (x2 each)
* MCP32017 i/o expander
* 8 DIP switch
  - Port swap
  - Behavior/Mode
  - Power source?? (USB v BAT) Just use a different charge module?
  - IP mode (Static/dynamic)
* status LEDs (bottom/hidden):
  - Power
  - Connected
  - Firmware updating/updated
* buttons/switches (bottom, internal):
  - Bootload Mode button
  - Reset button
  - Request OTA pdate from server?
  - Power switch
  - Pairing button
* LiPo (Usually 1-2x 18650 or 1000mAh flat (e.g. 063450)
  
## OSC Message Structure

|Sorting Header|Device Name|Component Level|Type Tags|Arguments|
|---           |---        |---|---|---|
|/inputModules |/name      |   |   |   |
|/outputModules|/name      |   |   |   |
|none          |/name      |   |   |   |
|none          |/name      |   |   |   |

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

### Outputs
Many of these would be best in multiples - or several slaves from one master.
* Baloon
* **RGB Ball**
* Linear Motor (fader motor or princess motor)
* LED Blinders
* **Bubble Machine**
* **Rover**
* Tethered Airplane
* String Rider
* Speaker cube (pregenerated set of simple beeps or honks or something)
* **Pan/Tilt head*** (set up for face track?)
* Antenna Motor

### Other
* OLED Display Sniffer + Message Translator (e.g. Touch OSC To devices)
  + Input buttons for mode etc.
  + Keyboard??? (assigning addresses)
  + This might be a Pi + display or something else more complex than just an ESP8266
  + DMX I/O??
