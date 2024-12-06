#include "WiFi_Helper.h"

WiFiHelper::WiFiHelper(const char* ssid, const char* password, uint8_t _pinStatus, bool _DEBUG) : _SSID(ssid), _PASSWORD(password), pinStatus(_pinStatus), DEBUG(_DEBUG){
  isConnected = false;
}

void WiFiHelper::wifiInit() {
    Serial.println("WiFi thread is working...");
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    pinMode(pinStatus, OUTPUT);
    digitalWrite(pinStatus, HIGH);
    if(DEBUG){
      Serial.print("[INFO] Connecting to: ");
      Serial.println(_SSID);
    }

    WiFi.begin(_SSID, _PASSWORD);
    if(DEBUG && WiFi.status() != WL_CONNECTED){
      Serial.print("[ERROR] not connected");
    }
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        if(DEBUG){
          Serial.print("-");
        }
    }
    isConnected = true;
    if(DEBUG){
      Serial.println("");
      Serial.println("[INFO] WiFi Connected");
    }
    digitalWrite(pinStatus, LOW);
    
}
