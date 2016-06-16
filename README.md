# TCT
The Tangible Characterization Toolkit for Tangible Interaction Design Inc. 

# TODO
[ ] Write TODO List

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

## Hardware Requirements/Wishlist (clients)
* ESP 12E
* MCP3208 ADC (x2 each)
* MCP32017 i/o expander
* 8 DIP switch
  - port swap
  - behavior/mode
  - power source?? (USB v BAT) Just use a different charge module?
  - ip mode (Static/dynamic)
* status LEDs (bottom/hidden):
  - power
  - connected
  - firmware updating/updated
* buttons/switches (bottom, internal):
  - Bootload Mode button
  - Reset Button
  - Check OTA Update Server?
  - Power switch
  
## OSC Message Structure

|Sorting Header|Device Name|Component Level|Type Tags|Arguments|
|---           |---        |---|---|---|
|/inputModules |/name      |   |   |   |
|/outputModules|/name      |   |   |   |
|none          |/name      |   |   |   |
|none          |/name      |   |   |   |
