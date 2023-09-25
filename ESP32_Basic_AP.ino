/*
 *** UTCN DMP LAB X ***
 * WiFi Part 1 - WiFi Access Point + HTTP Server with ESP32
 * originally adapted from https://github.com/espressif/arduino-esp32/blob/master/libraries/WiFi/examples/WiFiAccessPoint/WiFiAccessPoint.ino
 */

/*
 * HEADER FILES
 * Include header files provided by Espressif for the ESP32
 * https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFi/src
 */
#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>

/*
 * GLOBAL CONSTANTS & OTHER VARIABLES
 */

// MESSAGE STRINGS
const String SETUP_INIT = "SETUP: Initializing ESP32 dev board";
const String SETUP_ERROR = "!!ERROR!! SETUP: Unable to start SoftAP mode";
const String SETUP_SERVER_START = "SETUP: HTTP server started --> IP addr: ";
const String SETUP_SERVER_PORT = " on port: ";
const String INFO_NEW_CLIENT = "New client connected";
const String INFO_DISCONNECT_CLIENT = "Client disconnected";

// HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
// and a content-type so the client knows what's coming, then a blank line:
const String HTTP_HEADER = "HTTP/1.1 200 OK\r\nContent-type:text/html\r\n\r\n";
const String HTML_WELCOME = "<h1>Welcome to your ESP32 Web Server!</h1>";
const String HTML_TOGGLE_INFO = "<div>This URL toggles the built-in LED</div><br />";
const String HTML_TOGGLE_CHANGE = "<div>In order to change it, <span style=\"color: red\">please refresh this page in the browser</span></div>";


// BASIC WIFI CONFIGURATION CONSTANTS
// The SSID (Service Set IDentifier), in other words, the network's name
const char *SSID = "DMP_ESP32_AP";
// Password for the network
// By default the ESP32 uses WPA / WPA2-Personal security, therefore the
// the password MUST be between 8 and 63 ASCII characters
const char *PASS = "somepass2023";
// The default port (both TCP & UDP) of a WWW HTTP server number according to
// RFC1340 is 80
const int HTTP_PORT_NO = 80;

// ADDITIONAL GLOBALS
// Initialize the HTTP server on the ESP32 board
WiFiServer HttpServer(HTTP_PORT_NO);

/*
 * MAIN ARDUINO FUNCTIONS (setup & loop)
 */
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
  Serial.println();
  Serial.println(SETUP_INIT);

  // Initialize and the start the ESP32 board's WiFi module in SoftAP mode with
  // the network name and password from the beginning of the file
  if (!WiFi.softAP(SSID, PASS)) {
    Serial.println(SETUP_ERROR);
    // Lock system in infinite loop in order to prevent further execution
    while (1)
      ;
  }

  // Get AP's IP address for info message
  const IPAddress accessPointIP = WiFi.softAPIP();
  const String webServerInfoMessage = SETUP_SERVER_START + accessPointIP.toString()
                                      + SETUP_SERVER_PORT + HTTP_PORT_NO;

  // Start the HTTP server
  HttpServer.begin();
  Serial.println(webServerInfoMessage);
}

void loop() {
  WiFiClient client = HttpServer.available();  // listen for incoming clients

  if (client) {                        // if you get a client,
    Serial.println(INFO_NEW_CLIENT);   // print a message out the serial port
    String currentLine = "";           // make a String to hold incoming data from the client
    while (client.connected()) {       // loop while the client's connected
      if (client.available()) {        // if there's bytes to read from the client,
        const char c = client.read();  // read a byte, then
        Serial.write(c);               // print it out the serial monitor

        if (c == '\n') {  // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // Send welcome page to the client
            printWelcomePage(client);
            break;
          } else currentLine = "";
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Send toggle built-in LED page to the client
        if (currentLine.endsWith("GET /toggle")) {
          Serial.println();
          printToggle(client);
          break;
        }

        // INDIVIDUAL WORK TO IMPLEMENT READING FROM SENSOR
        // Modify in web_functions.cpp
        if (currentLine.endsWith("GET /read-sens")) {
          Serial.println();
          printReadSensor(client);
          break;
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println(INFO_DISCONNECT_CLIENT);
    Serial.println();
  }
}

/*
 * ADDITIONAL HELPER FUNCTIONS
 */
void printWelcomePage(WiFiClient client) {
  // Always start the response to the client with the proper headers
  client.println(HTTP_HEADER);

  // Send the relevant HTML
  client.print(HTML_WELCOME);

  // The HTTP response ends with another blank line
  client.println();
}

void printToggle(WiFiClient client) {
  // Get currently elapsed time
  const unsigned long elapsedTime = millis();

  // Get value from LED and flip it on the dev board
  const bool newVal = !digitalRead(LED_BUILTIN);
  digitalWrite(LED_BUILTIN, newVal);
  const String currState = newVal == 1 ? String("ON") : String("OFF");

  // Send the HTML code to the client
  client.println(HTTP_HEADER);
  client.print(HTML_TOGGLE_INFO);
  const String stateMessage = String("Currently, it is <strong>")
                              + currState + String("</strong>");
  client.print(String("<div>") + stateMessage + String("</div><br />"));
  client.print(String("<div>It was last toggled <i>") + String(elapsedTime / 1000)
               + String(" seconds</i> after the start of the application</div><br />"));
  client.print(HTML_TOGGLE_CHANGE);
  client.println();
}

// TODO: Implement reading from sensor and display value
void printReadSensor(WiFiClient client) {}