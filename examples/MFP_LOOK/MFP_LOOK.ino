#include <WiFi.h>
#include <ESPmDNS.h>
#include "ArduinoMFP.h"

const char* ssid = "YourSSID";
const char* password = "YourPassword";

ArduinoMFP mfp;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");

  if (!MDNS.begin("esp32-mfp")) {
    Serial.println("Error setting up MDNS responder!");
    return;
  }

  delay(1000);  // Wait a bit before scanning

  Serial.println("\n=== LOOKING FOR PRINTERS ===");
  String printersJson = mfp.look(0);
  Serial.println(printersJson);

  Serial.println("\n=== LOOKING FOR SCANNERS ===");
  String scannersJson = mfp.look(1);
  Serial.println(scannersJson);
}

void loop() {
  // Nothing in loop for this test
}
