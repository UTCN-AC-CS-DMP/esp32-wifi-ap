/*
 * UTCN LAB X - WiFi with ESP32
 * originally adapted from https://github.com/espressif/arduino-esp32/blob/master/libraries/WiFi/examples/WiFiAccessPoint/WiFiAccessPoint.ino
 */

// Include header files from ESP32
// https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFi/src
#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>

// Include our own project header files
#include "main_constants.h"
#include "web_functions.h"

// Initialize the HTTP server on the ESP32 board
WiFiServer HttpServer(HTTP_PORT_NO);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);

// ESP32 boards have a delay with UART for some reason from a reset after
// uploading new code to the development board
#if 1  // board was newly programmed
  delay(1000);
#else
  // https://www.arduino.cc/reference/en/language/functions/communication/serial/ifserial/
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB
  }
#endif
  Serial.println(INFO_INIT);

  // Initialize and the start the ESP32 board's WiFi module in SoftAP mode with
  // the network name and password from the beginning of the file
  if (!WiFi.softAP(SSID, PASS)) {
    Serial.println(ERROR_INIT);
    // Lock system in infinite loop in order to prevent further execution
    while (1)
      ;
  }

  // Get AP's IP address for info message
  const IPAddress ACCESS_POINT_IP = WiFi.softAPIP();
  const String WEB_SERVER_INFO = String(String("SETUP: HTTP server started --> IP addr: ") + ACCESS_POINT_IP.toString() + String(" on port: ") + HTTP_PORT_NO);

  // Start the HTTP server
  HttpServer.begin();

  Serial.println(WEB_SERVER_INFO);
}

void loop() {
  WiFiClient client = HttpServer.available();  // listen for incoming clients

  if (client) {                     // if you get a client,
    Serial.println("New Client.");  // print a message out the serial port
    String currentLine = "";        // make a String to hold incoming data from the client
    while (client.connected()) {    // loop while the client's connected
      if (client.available()) {     // if there's bytes to read from the client,
        char c = client.read();     // read a byte, then
        Serial.write(c);            // print it out the serial monitor
        if (c == '\n') {            // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            printWelcomePage(client);
            break;  // break out of the while loop
          } else {  // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        if (currentLine.endsWith("GET /toggle")) {
          printToggle(client);
          break;
        }

        // INDIVIDUAL WORK TO IMPLEMENT READING FROM SENSOR
        // Modify in web_functions.cpp
        if (currentLine.endsWith("GET /read-sens")) {
          printReadSensor(client);
          break;
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
