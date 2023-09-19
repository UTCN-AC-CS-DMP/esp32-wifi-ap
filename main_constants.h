/*
 * MESSAGE STRINGS
 */

const String INFO_INIT = "SETUP: Initializing ESP32 dev board";
const String ERROR_INIT = "!!ERROR!! SETUP: Unable to start SoftAP mode";

/*
 * BASIC CONFIGURATION CONSTANTS 
 */

// The SSID (Service Set IDentifier), in other words, the network's name
const char *SSID = "DMP_ESP32_AP";

// Password for the network
// By default the ESP32 uses WPA / WPA2-Personal security, therefore the
// the password MUST be between 8 and 63 ASCII characters
const char *PASS = "somepass2023";

// The default port (both TCP & UDP) of a WWW HTTP server number according to
// RFC1340 is 80
const int HTTP_PORT_NO = 80;
