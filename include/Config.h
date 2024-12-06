#ifndef CONFIG_H
#define CONFIG_H

#include "pinMapping.h"
#include "../src/ESP32Thread/ESP32Thread.h"
#include "../src/WiFi Helper/WiFi_Helper.h"
#include "../src/MAX30102 Helper/MAX30102_Helper.h"
#include "../src/Firebase Helper/Firebase_Helper.h"
#include "../src/D6T/D6T.h"


// ****            Patient Data           **** //
#define Name "Ahmed Ibrahim"
#define Age 23
#define EMG_C "01558200123"
// ******************************************* //

// ****         WiFi and Firebase         **** //
#define WIFI_SSID "Shabrawii@"
#define WIFI_PASS "128932ac"
#define API_KEY "AIzaSyA2C_vxzilFEbvJclgZPaLT3aw1EL5xrzg"
#define USER_EMAIL "monitor_0001@gmail.com"
#define USER_PASSWORD "128932ac"
#define DATABASE_URL "https://centeralized-vitals-monitor-default-rtdb.europe-west1.firebasedatabase.app/"
// ******************************************* //

// ****           ESP32 Threads           **** //
ESP32Thread WiFi_Thread(1000);
ESP32Thread MAX30102_Thread(10000);
// ******************************************* //

//**                OBJECTS                ** //
WiFiHelper wifiHelper(WIFI_SSID, WIFI_PASS, WIFI_STATUS_Pin, __DEBUG__);
MAX30102Controller max30102Controller(MAX30102_STATUS_Pin, MAX30102_pulseLED_Pin, MAX30102_readLED_Pin, __DEBUG__);
FirebaseHelper firebaseHelper(API_KEY, USER_EMAIL, USER_PASSWORD, DATABASE_URL);

D6T d6t;

// ******************************************* //

#endif