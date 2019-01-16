# Count.It Neon IOT project

This IOT project was made to control the Count.it Neon light logo depending on different threshold values 

# Required Hardware

* Arduino UNO WIFI REV2
* Neon Power Supply (Model: GEN3 - 8K20)
* Count.it Neon logo

# Known Issues with fix:

* Arduino Uno does not use wifi101 library it uses WifiNina library (So search mkr1000 code won't work)
* When making http GET requests you may get weird errors - Best to do in this case is to update the firmware of the arduino using this tutorial: https://forum.arduino.cc/index.php?topic=579469.0 (Some of the errors may be HTTP error code 301)
* Also when making an HTTPS request, in other words a server with SSL make sure to use the WiFiSSLClient object instead and port 443.

# References

1. Power supplies
	* http://www.t2-neonpower.com/PRODUCTS/NEON_SUPPLIES/8k20_120V_files/8k20_120V.html
