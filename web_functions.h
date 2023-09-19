#ifndef _WEB_FUNCTIONS_H_
#define _WEB_FUNCTIONS_H_

#include <WiFiClient.h>

// HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
// and a content-type so the client knows what's coming, then a blank line:
const String HTTP_HEADER = "HTTP/1.1 200 OK\r\nContent-type:text/html\r\n\r\n";

const String HTML_WELCOME = "<h1>Welcome to your ESP32 Web Server!</h1>";

void printWelcomePage(WiFiClient client);
void printToggle(WiFiClient client);
void printReadSensor(WiFiClient client);

#endif _WEB_FUNCTIONS_H_