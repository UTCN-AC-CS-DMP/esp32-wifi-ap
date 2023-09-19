#include "web_functions.h"

void printWelcomePage(WiFiClient client) {
  // Always start the response to the client with the proper headers
  client.println(HTTP_HEADER);

  // Send the relevant HTML
  client.print(HTML_WELCOME);

  // The HTTP response ends with another blank line
  client.println();
}

void printToggle(WiFiClient client) {
  // Get value from LED and flip it on the dev board
  bool newVal = !digitalRead(LED_BUILTIN);
  digitalWrite(LED_BUILTIN, newVal);
  const String CURR_STATE = newVal == 1 ? String("ON") : String("OFF");

  // Send the HTML to the client
  client.println(HTTP_HEADER);
  client.print("<div>This URL toggles the built-in LED</div><br />");
  const String STATE_MESS = String("Currently, it is ") + CURR_STATE;
  client.print(String("<div>") + STATE_MESS + String("</div><br />"));
  client.print("<div>In order to change it, please refresh this page</div>");
  client.println();
}

// TODO: Implement reading from sensor and display value
void printReadSensor(WiFiClient client) {

}