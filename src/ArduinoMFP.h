#ifndef ArduinoMFP_h
#define ArduinoMFP_h

#include <Arduino.h>
#include <WiFi.h>

class ArduinoMFP {
public:
    ArduinoMFP();
    ~ArduinoMFP();

    // Scan method returns pointer to image data or nullptr if failed
    uint8_t* scan(int height, int width, const char* origin, const char* url, int port, const char* format, int filesystem);
    size_t getImageSize() const;
    uint8_t* getImageBuffer() const;
    String look(int mode);
    String print(const char* url, int port, String payload);
    String supported(const char* url, int port);

private:
    uint8_t* imageBuffer;
    size_t imageSize;

    String generateUUID();
    String buildCreateScanJobSOAP(const String& messageID, const String& jobUUID, int height, int width, const String& origin, const String& format);
    String buildRetrieveImageSOAP(const String& messageID, const String& jobUUID, const String& jobId, const String& jobToken);
    String sendSoapRequest(const String& host, int port, const String& soapBody);
    bool createScanJob(const String& host, int port, int height, int width, const String& origin, const String& format, String& jobUUID, String& jobId, String& jobToken);
    bool retrieveImage(const String& host, int port, const String& jobUUID, const String& jobId, const String& jobToken);

    String extractTag(const String& data, const String& tag);

    void freeImageBuffer();
};

#endif
