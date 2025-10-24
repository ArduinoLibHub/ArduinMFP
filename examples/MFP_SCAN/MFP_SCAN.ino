#include <WiFi.h>
#include "ArduinoMFP.h"
#include <SPIFFS.h>
// #include <LittleFS.h>   // Optional alternative if you prefer LittleFS

ArduinoMFP mfp;

void setup() {
  Serial.begin(115200);
  WiFi.begin("YourSSID", "YourPassword");

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi connected");

  // --- Try to mount SPIFFS ---
  bool spiffsReady = SPIFFS.begin(true);
  if (!spiffsReady) {
    Serial.println("⚠️ SPIFFS mount failed — continuing in RAM-only mode");
  }

  // --- Attempt SPIFFS scan first if mounted ---
  if (spiffsReady) {
    Serial.println("📂 Scanning and saving to SPIFFS...");
    uint8_t* img = mfp.scan(100, 100, "ADF", "_Your scanner's ip address_", 80, "exif", 0);
    if (img) {
      Serial.println("✅ Scan completed and saved to SPIFFS as /scan.jpg");
      return; // done successfully
    } else {
      Serial.println("❌ SPIFFS scan failed — retrying in RAM-only mode...");
    }
  }

  // --- RAM fallback if SPIFFS missing or failed ---
  Serial.println("🧠 Running scan in RAM-only mode...");
  uint8_t* img = mfp.scan(100, 100, "ADF", "_Your scanner's ip address_", 80, "exif", -1);
  if (img) {
    size_t size = mfp.getImageSize();
    Serial.printf("✅ Scan successful (RAM mode), image size: %d bytes\n", (int)size);
    // Access the in-memory buffer directly:
    // uint8_t* data = mfp.getImageBuffer();
  } else {
    Serial.println("❌ Scan failed completely.");
  }
}

void loop() {
  // Nothing here for now
}
