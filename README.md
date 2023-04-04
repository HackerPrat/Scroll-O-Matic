# Scroll-O-Matic

This is an Arduino sketch that allows you to create a web server on an ESP32 board with an OLED screen attached. Users can connect to the server and input text which will be displayed in a scrolling marquee on the OLED screen.

## Hardware Requirements
- ESP32 board
- OLED screen (128x64 pixels) connected to I2C interface
- USB cable

## Software Requirements
- Arduino IDE
- WiFi.h library
- WebServer.h library
- Wire.h library
- Adafruit_GFX.h library
- Adafruit_SSD1306.h library

## Setup Instructions
1. Install the Arduino IDE and necessary libraries
2. Connect the OLED screen to the ESP32 board using the I2C interface
3. Open the sketch in the Arduino IDE
4. Modify the `WiFi.softAP()` function to set the access point name as desired
5. Upload the sketch to the ESP32 board
6. Open the Serial Monitor to view the IP address assigned to the ESP32 access point
7. Connect a device to the ESP32 access point and navigate to the IP address in a web browser
8. Enter text in the input field and click submit to display the scrolling marquee on the OLED screen

## Code Overview
The sketch initializes the OLED screen and web server on the ESP32 board. Users can access the server by connecting to the ESP32 access point and navigating to the assigned IP address in a web browser. The server displays a form with an input field for users to enter text. The submitted text is received by the server and displayed in a scrolling marquee on the OLED screen.

The scrolling marquee displays the entered text and moves upwards until it disappears off the top of the screen. Once the marquee has reached the top, it resets to the bottom of the screen and starts again. The scrolling speed can be modified by adjusting the `textSpeed` variable.

## Credits
This code is based on the work of Adafruit Industries and their SSD1306 OLED driver library and GFX library. The web server is based on the WebServer library for ESP32 by Espressif.
