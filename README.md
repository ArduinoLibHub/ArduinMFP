# 🖨️ ArduinoMFP Library  
**Multi-Function Printer/Scanner SOAP Interface for ESP32 (and similar Wi-Fi boards)**

## 📘 Overview
`ArduinoMFP` is an Arduino C++ library designed to control and interact with **network multifunction printers (MFPs)** — specifically those that support **WSD, eSCL, or AirScan protocols** — over Wi-Fi.  

It allows an ESP32 (or other Wi-Fi-enabled MCU) to:
- Discover network printers and scanners via **mDNS (Bonjour/ZeroConf)**
- **Scan documents** and optionally save them to SPIFFS or LittleFS
- **Send print jobs** via raw TCP socket
- **Query device metadata** (model name, supported services, URLs)

The library wraps SOAP (Simple Object Access Protocol) XML messages for WSD printers/scanners, enabling full control without external libraries.

---

## ⚙️ Features
- 🔍 **Device Discovery:** Search for available printers/scanners via mDNS  
- 📠 **Scanning Support:** Initiate scan jobs and retrieve JPEG data  
- 💾 **Filesystem Output:** Save scanned images to SPIFFS or LittleFS  
- 🧾 **Printing Support:** Send plain text payloads to TCP-based printers  
- 🧩 **Metadata Fetching:** Retrieve printer model info and service URLs via WSD SOAP  
- 🔐 **Self-managed Memory:** Automatically allocates and frees scan buffers  

---

## 📦 Installation
1. Copy both files into a new folder:
   ```
   Arduino/libraries/ArduinoMFP/
   ```
   containing:
   - `ArduinoMFP.cpp`
   - `ArduinoMFP.h`
2. In the Arduino IDE, include the library:
   ```cpp
   #include <ArduinoMFP.h>
   ```

3. Make sure your board supports:
   - Wi-Fi (`<WiFi.h>`)
   - mDNS (`<ESPmDNS.h>`)
   - SPIFFS or LittleFS

---

## 🚀 Usage

### 1️⃣ Initialization
```cpp
#include <ArduinoMFP.h>

ArduinoMFP mfp;  // Create a library instance
```

---

### 2️⃣ Discover Printers or Scanners
```cpp
// mode = 0 → Printers, 1 → Scanners
String found = mfp.look(1);
Serial.println(found);
```
Returns a JSON-like string:
```json
{"scanners":[{"host":"BrotherDCP","ip":"172.20.8.35","port":80}]}
```

---

### 3️⃣ Query Device Metadata
```cpp
String info = mfp.supported("172.20.8.35", 80);
Serial.println(info);
```
Example output:
```json
{
  "modelName": "Brother DCP-L2540DW series",
  "modelUrl": "http://www.brother.com",
  "printerService": "http://172.20.8.35:80/WebServices/PrinterService",
  "scannerService": "http://172.20.8.35:80/WebServices/ScannerService"
}
```

---

### 4️⃣ Scan a Document
```cpp
uint8_t* image = mfp.scan(
  300, 300,           // resolution height/width
  "Platen",           // source (Platen or Feeder)
  "172.20.8.35", 80,  // device IP and port
  "image/jpeg",       // format
  0                   // filesystem: 0=SPIFFS, 1=LittleFS, -1=no save
);

if (image != nullptr) {
  Serial.printf("Scan complete! Size: %d bytes\n", mfp.getImageSize());
}
```

This automatically saves `/scan.jpg` if SPIFFS or LittleFS is chosen.

---

### 5️⃣ Print Text
```cpp
String response = mfp.print("172.20.8.35", 9100, "Hello from ESP32!");
Serial.println(response);
```

---

## 🧠 Class Summary

| Method | Description |
|--------|--------------|
| `uint8_t* scan(int h, int w, const char* origin, const char* url, int port, const char* format, int filesystem)` | Starts a scan job and retrieves JPEG image. |
| `size_t getImageSize()` | Returns the number of bytes in the current image buffer. |
| `uint8_t* getImageBuffer()` | Returns pointer to raw image data. |
| `String look(int mode)` | Discovers available printers or scanners using mDNS. |
| `String print(const char* ip, int port, String payload)` | Sends a print job. |
| `String supported(const char* url, int port)` | Fetches model and service metadata using WSD SOAP. |

---

## 💾 Filesystem Options

| Value | Description |
|--------|-------------|
| `0` | Save scan to SPIFFS (`/scan.jpg`) |
| `1` | Save scan to LittleFS (`/scan.jpg`) |
| `-1` | Do not save (keep in memory only) |

---

## 🧩 Internal Helpers

| Private Method | Role |
|----------------|------|
| `generateUUID()` | Generates random UUIDs for SOAP requests. |
| `buildCreateScanJobSOAP()` | Builds XML for scan job creation. |
| `buildRetrieveImageSOAP()` | Builds XML for image retrieval. |
| `sendSoapRequest()` | Sends generic SOAP requests via Wi-Fi. |
| `extractTag()` | Extracts XML tag values. |
| `freeImageBuffer()` | Frees allocated scan memory safely. |

---

## 🧰 Requirements
- **Board:** ESP32 / ESP8266 (with Wi-Fi & mDNS)
- **Libraries:**
  - `WiFi.h`
  - `ESPmDNS.h`
  - `FS.h`
  - `SPIFFS.h`
  - `LittleFS.h`

---

## 🧪 Example Output
```
📡 Searching for scanners...
{"scanners":[{"host":"BrotherDCP","ip":"172.20.8.35","port":80}]}

📘 Querying device info...
{"modelName": "Brother DCP-L2540DW series", "printerService": "...", "scannerService": "..."}

📠 Starting scan...
Image saved to filesystem (409632 bytes)
```

---

## 🧑‍💻 Example Sketch
```cpp
#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <SPIFFS.h>
#include <ArduinoMFP.h>

const char* ssid = "YourSSID";
const char* pass = "YourPassword";

ArduinoMFP mfp;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) delay(500);

  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS init failed!");
    return;
  }

  Serial.println(mfp.supported("172.20.8.35", 80));
  mfp.scan(300, 300, "Platen", "172.20.8.35", 80, "image/jpeg", 0);
}

void loop() {}
```

---

## ⚠️ Notes
- This library uses **SOAP over HTTP** (no HTTPS).
- Tested with **Brother DCP-L2540DW** and similar models.
- Image transfer is **blocking** — best run in isolated tasks.
- Ensure scanner supports **WSD/ScanToPC**.

---

## 📄 License

MIT License

Copyright (c) 2025 Duke Uku

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

