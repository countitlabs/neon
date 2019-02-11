# Count.It Neon IOT project

This IOT project was made to control the Count.it Neon light logo depending on different fitness score values from the count.it api.

# Required Hardware

* Arduino UNO WIFI REV2
* Neon Power Supply (Model: GEN3 - 8K20)
* Count.it Neon logo

# Libraries Needed

* ArduinoJson Version: 5.13.2
* WifiNina Version: 1.3.0

# Known Issues with fix:

* Arduino Uno does not use wifi101 library it uses WifiNina library (So search mkr1000 code won't work)
* Also when making an HTTPS request, in other words a server with SSL make sure to use the WiFiSSLClient object instead and port 443.
* Make sure to install the Hourly build for the arduino ide for best solutions!
* When making http GET requests you may get weird errors - Best to do in this case is to update the firmware of the arduino using this tutorial: https://forum.arduino.cc/index.php?topic=579469.0 (Some of the errors may be HTTP error code 301)

# References

1. Power supplies
	* http://www.t2-neonpower.com/PRODUCTS/NEON_SUPPLIES/8k20_120V_files/8k20_120V.html
2. Arduino Json assistant
	* https://arduinojson.org/v5/assistant/
3. Arduino Uno Wifi Rev2 Docs
	* https://store.arduino.cc/usa/arduino-uno-wifi-rev2
4. Count.It Api
	* https://www.countit.com/api/office/6f434034-040f-4ac8-b2c6-f19585485b2c/score

# Documentation:

## Api.h : Allows Api GET requests
## control_sequence.h : Controls all light sequences
## light_control.h : Controls pin OutPut logic depending on score values
## pwm.h : Controls Pulse Width Modulation signals to the transformers (High and Low)
## access_point.h : Controls access point generation and wifi connection
## main.h : Controls all main logic

# To Do:

### Investigate wireslesly uploading of code to arduino
### Save user variables for 5 min intervals everytime the device is shut off - Used so that the user does not need to re enter values unless needed. (eeprom_main.cpp and eeprom_saving.cpp work for Arduino Uno device - Not for Arduino Uno wifi rev2) 
