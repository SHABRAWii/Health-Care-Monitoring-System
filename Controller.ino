#define __DEBUG__ false


#include <Arduino.h>
#include "include/Config.h"

// ESP32Thread TEST(1000);

// ** TEST ** //


// ** TEST ** //

bool _once = 1;

void setup() {
  Serial.begin(115200);
  WiFi_Thread.begin([](){
      wifiHelper.wifiInit();
  });
  MAX30102_Thread.begin([](){
    max30102Controller.loop();
  });
  while(!wifiHelper.isConnected){
    Serial.print('.');
    delay(1000);
  }
  Serial.println("Finished setup and connected");
}
void loop() {
  firebaseHelper.app.loop();
  firebaseHelper.database.loop();
    if (firebaseHelper.app.ready() && firebaseHelper.aClient->taskCount() < 2)
    {
      if (_once){
        firebaseHelper.pushString(String(Name) + "/Age", String(Age));
        firebaseHelper.pushString(String(Name) + "/Emergency Contact", String(EMG_C));
        _once = 0;
      }
      if(firebaseHelper.tmpData[1] != max30102Controller.BPM){
        firebaseHelper.tmpData[1] = max30102Controller.BPM;
        firebaseHelper.pushFloat(Name + String("/MAX30102/HeartRate"), max30102Controller.BPM);
      }
      double ptat;
      double pix_data[N_PIXEL];
      if (d6t.readTemperature(ptat, pix_data)) {
        if(firebaseHelper.tmpData[2] != ptat){
          firebaseHelper.tmpData[2] = ptat;
          firebaseHelper.pushFloat(Name + String("/Body Temperature"), ptat);
        }
      } 
      if(firebaseHelper.tmpData[1] != max30102Controller.beer){
        firebaseHelper.tmpData[1] = max30102Controller.beer;
        firebaseHelper.pushFloat(Name + String("/MAX30102/beer"), max30102Controller.beer);
      }
    //   // if(firebaseHelper.tmpData[4] != max30102Controller.spo2){
    //   //   firebaseHelper.tmpData[4] = max30102Controller.spo2;
    //   //   firebaseHelper.pushFloat(Name + String("/MAX30102/SPO2") + String(max30102Controller.spo2) + ";");
    //   // }
    //   // if(firebaseHelper.tmpData[5] != max30102Controller.status){
    //   //   firebaseHelper.tmpData[5] = max30102Controller.status;
    //   //   firebaseHelper.pushFloat(Name, "Status:" + String(max30102Controller.status) + ";");
    //   // }

    }

    // firebaseHelper.printResult(firebaseHelper.aResult_no_callback);
}

