#ifndef FIREBASE_HELPER_H
#define FIREBASE_HELPER_H

#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseClient.h>
#include <WiFiClientSecure.h>

using AsyncClient = AsyncClientClass;

class FirebaseHelper {
public:
    FirebaseHelper(const char* apiKey, const char* email, const char* emailPassword, const char* databaseURL);
    void Init();
    void loop();
    void pushInt(const char* path, int value);
    void pushBool(const char* path, bool value);
    void pushFloat(String path, float value);
    void pushString(String path, String value);

    uint32_t tmpData[20];
    FirebaseApp app;
    RealtimeDatabase database;
    bool taskComplete;
    
    AsyncClient* aClient;
    void printResult(AsyncResult &aResult);
    AsyncResult aResult_no_callback;
private:
    WiFiClientSecure ssl_client;
    DefaultNetwork network;
    UserAuth* user_auth;
};

#endif
