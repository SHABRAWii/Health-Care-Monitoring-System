#ifndef WIFI_HELPER_H
#define WIFI_HELPER_H

#include <cstdint>
#include <WiFi.h>

class WiFiHelper {
public:
    WiFiHelper(const char* ssid, const char* password, uint8_t _pinStatus, bool _DEBUG = 0);
    void wifiInit();
    bool isConnected;
private:
    const char* _SSID; // Declare private member variables
    const char* _PASSWORD;
    bool DEBUG;
    uint8_t pinStatus;
};

#endif
