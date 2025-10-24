#include <WiFi.h>
#include "ArduinoMFP.h"

// Replace with your network credentials
const char* ssid = "YourSSID";
const char* password = "YourPASSWORD";

// Printer IP and port (usually 9100 for raw printing)
const char* printerIP = "Your printers ip address.";  
const int printerPort = 9100;

ArduinoMFP mfp;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  String payload = "Whatever you want to print.";

  Serial.println("Sending print job...");
  String response = mfp.print(printerIP, printerPort, payload);

  Serial.println("Printer response:");
  Serial.println(response);
}

void loop() {
  // Nothing here
}
