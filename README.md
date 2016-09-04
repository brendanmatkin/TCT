# TCT
The Tangible Characterization Toolkit for Tangible Interaction Design Inc. 

# Message Structure:
**standard message:** address | xVal(f) | yVal(f) | zVal(f) | x16(i) 
  notes: 
  * x-, y-, zVal are all -1.0f to 1.0f
  * 16 ints belong to either toSend[] or received[]
  * address is still being finalized... see below 

# TODO
- [ ] Config File
- [x] Status LEDs (ws2812)
- [ ] finish status logic & LED patterns
- [x] DIP switch
- [ ] TEST integration procedure w/ existing system(s)
- [x] OSC Receive (**message** or ~~bundle?~~ - no bundle support at this time)
- [x] Send packets only on value change! (this is also scheduled - check for change every 10 ms)
- [ ] Two modules talking without user input (or pair button)
- [ ] Make ESP mini router for travel/convenience testing (TCT AP, 10.0.2.254). 
- [x] Determine and order batteries (usually 063450 or similar 1000mAh LiPos)
- [ ] ESP-based OTA sketch server
- [ ] SPIFFS config file
- [x] OSC Reports (this exists but isn't really used anywhere yet)
- [x] ESP internal ADC to measure battery voltage (prevent over-discharge on LiPo w/o protection circuit - also sends in OSC report)
- [ ] Write TODO List (ongoing)

## Software Requirements/Wishlist (general)
- [x] OSC for data exchange and status broadcast
- [x] Multicast UDP (239.0.0.100:7777 or :3333)
- [x] Only send packets when there is new information (e.g. over a certain threshold of change)
- [x] Static IPs (for firmware updates, debug, etc)
- [ ] OTA Updates via SD card ESP Server
- [ ] m/DNS-SD (nice but not required)
- [x] TickerScheduler Library
- [ ] JSON (file system - config files)
- [x] WiFi.RSSI() reported on OSC status (currently in own message)
- [x] ADC Battery level check (and report) - ~~Alternate: Fuel Guage IC~~
- [ ] Store DEVICE NAME (e.g. TCT01, TCT02, etc.) in EEPROM (easier conditionals on firmware updates?)
- [ ] Pairing mode! (hold a button to auto pair? - button is in generic module, but no code yet)
- [ ] ~~Consider timetag usage, if at all (millis since startup? Probably not necessary?)~~

## Hardware Requirements/Wishlist (clients)
- [x] ESP 12E
- [x] MCP3208 ADC (x2 in some)
- [x] MCP32017 i/o expander (maybe x2 in some)
- [x] 8 DIP switch
  - Port swap
  - Behavior/Mode
  - Power source?? (USB v BAT) Just use a different charge module?
  - IP mode (Static/dynamic)
  - on-board smoothing/LPF (EWMAverage probably)
  - TUIO/OSC standard (this can come later - requires some thinking)
- [x] Status LEDs (bottom/hidden) (ws2812B):
  - Power
  - Connected
  - Firmware updating/updated
- [x] Buttons/switches (bottom or internal):
  - Bootload button
  - Reset button
  - Request OTA update from server (hold down bootload for x seconds)?
  - Power switch
  - Pairing button ?? (determine this behavior)
- [x] LiPo (Usually 1-2x 18650 or ~1000mAh flat (e.g. 063450)

## OTA Server Requirements/Wishlist (Over The Air Firmware & SPIFFS (file system) update server)
- [ ] ESP 12E
- [ ] SD Card Slot
- [ ] Some buttons (not sure what is needed from this yet!)
  
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
- [x] **IR Proximity Array**
- [ ] **Accelerometer** (gravity measure? Gravity + linear)
- [ ] Finger Holes (touch, proximity, photo interrupt, etc)
- [ ] 38kHz IR (i.e. fireflys + darktorch - requires multiple modules; Zoomable darktorch?)
- [x] **Joystick**
- [ ] Touch Panel (e.g. DS touch layer - no screen)
- [ ] Face tracker (Camera Vision, likely just USB + simple Procesing app to analyze & join the multicast group)
- [ ] Button(s) (like a button array to match IR Array?)
- [ ] **squishy surface**
- [ ] blow sensor
- [ ]

### Outputs
Many of these would be best in multiples - or several slaves from one master.
- [ ] Balloon
- [ ] **Linear Motor** (fader motor or princess motor) (do this in multiples)
- [ ] Bubble Machine
- [ ] **Rover**
- [ ] **Tethered Airplane**
- [ ] String Rider?????
- [ ] Speaker cube (pregenerated set of simple beeps or honks or something)
- [ ] **Pan/Tilt head** - generic mount (stick w/ LED, face, etc. Must be strong.)
- [ ] **Antenna Motor** (just build this one and make it interesting later)
- [ ] Audio Trigger (plays more complex audio e.g. binaural clips, music, PD/Max bridge)
- [ ] mini halo motion - opening flower, a bendy 'nose' (pan/tilt) -- flexible 3D print
- [ ] super tiny demo (oled probably)
- [ ] rapid inflatable + _____?
- [ ] silcone inflation dome (like pneumatible)
- [ ] thermal printer something...

### Other
* OLED Display Sniffer + Message Translator (e.g. Touch OSC To devices)
  + Input buttons for mode etc.
  + Keyboard??? (assigning addresses)
  + This might be a Pi + display or something else more complex than just an ESP8266
  + DMX I/O??
