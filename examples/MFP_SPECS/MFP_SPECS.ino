#include <WiFi.h>
#include "ArduinoMFP.h"

const char* ssid = "YourSSID";
const char* password = "YourPASSWORD";

// Target scanner/printer info
const char* deviceIP = "The ip address"; // replace with your actual IP
const int devicePort = 80;              // most often 80 or 8080

ArduinoMFP mfp;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\nConnecting to WiFi...");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());

  // Call the metadata fetcher
  Serial.println("Querying device metadata...");
  String info = mfp.supported(deviceIP, devicePort);
  Serial.println("Result:");
  Serial.println(info);
}

void loop() {
  // Do nothing
}
