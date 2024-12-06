#include "MAX30102_Helper.h"

MAX30102Controller::MAX30102Controller(uint8_t _pinStatus, uint8_t _pulseLED, uint8_t _readLED, bool DEBUG)
 : pinStatus(_pinStatus), pulseLED(_pulseLED), readLED(_readLED)  {
  if(DEBUG){
    // #define __DEBUG__ true
  }
}

MAX30102Controller::~MAX30102Controller() {}


void MAX30102Controller::configureMAX30100()
{
    pinMode(pinStatus, OUTPUT);
    digitalWrite(pinStatus, HIGH);
    pinMode(pulseLED, OUTPUT);
    pinMode(readLED, OUTPUT);
    status = particleSensor.begin(Wire, I2C_SPEED_FAST);
    if(!status)
      return;
    byte ledBrightness = 60; //Options: 0=Off to 255=50mA
    byte sampleAverage = 4;  //Options: 1, 2, 4, 8, 16, 32
    byte ledMode = 2;        //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
    byte sampleRate = 100;   //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
    int pulseWidth = 411;    //Options: 69, 118, 215, 411
    int adcRange = 4096;     //Options: 2048, 4096, 8192, 16384

    digitalWrite(pinStatus, LOW);
    // particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings
    particleSensor.setup();
    particleSensor.setPulseAmplitudeRed(0x0A); // Turn Red LED to low to indicate sensor is running
    particleSensor.setPulseAmplitudeGreen(0); // Turn off Green LED
}
void MAX30102Controller::algorithm_1(){
  bufferLength = 100; //buffer length of 100 stores 4 seconds of samples running at 25sps

  //read the first 100 samples, and determine the signal range
  for (byte i = 0 ; i < bufferLength ; i++)
  {
    while (particleSensor.available() == false) //do we have new data?
      particleSensor.check(); //Check the sensor for new data

    redBuffer[i] = particleSensor.getRed();
    irBuffer[i] = particleSensor.getIR();
    particleSensor.nextSample(); //We're finished with this sample so move to next sample

    #ifdef __DEBUG__ 
      Serial.print(F("red="));
      Serial.print(redBuffer[i], DEC);
      Serial.print(F(", ir="));
      Serial.println(irBuffer[i], DEC);
    #endif
  }

  //calculate heart rate and SpO2 after first 100 samples (first 4 seconds of samples)
  maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);

  //Continuously taking samples from MAX30102.  Heart rate and SpO2 are calculated every 1 second
  while (1)
  {
    //dumping the first 25 sets of samples in the memory and shift the last 75 sets of samples to the top
    for (byte i = 25; i < 100; i++)
    {
      redBuffer[i - 25] = redBuffer[i];
      irBuffer[i - 25] = irBuffer[i];
    }

    //take 25 sets of samples before calculating the heart rate.
    for (byte i = 75; i < 100; i++)
    {
      while (particleSensor.available() == false) //do we have new data?
        particleSensor.check(); //Check the sensor for new data

      digitalWrite(readLED, !digitalRead(readLED)); //Blink onboard LED with every data read

      redBuffer[i] = particleSensor.getRed();
      irBuffer[i] = particleSensor.getIR();
      particleSensor.nextSample(); //We're finished with this sample so move to next sample

      #ifdef __DEBUG__
        Serial.print(F("red="));
        Serial.print(redBuffer[i], DEC);
        Serial.print(F(", ir="));
        Serial.print(irBuffer[i], DEC);

        Serial.print(F(", HR="));
        Serial.print(heartRate, DEC);

        Serial.print(F(", HRvalid="));
        Serial.print(validHeartRate, DEC);

        Serial.print(F(", SPO2="));
        Serial.print(spo2, DEC);

        Serial.print(F(", SPO2Valid="));
        Serial.print(validSPO2, DEC);
      #endif
      
      //** BPM **//

      // if (checkForBeat(irBuffer[i]) == true) {
      //     long delta = millis() - lastBeat;
      //     lastBeat = millis();

      //     beatsPerMinute = 60 / (delta / 1000.0);

      //     if (beatsPerMinute < 255 && beatsPerMinute > 20) {
      //         rates[rateSpot++] = (byte)beatsPerMinute;
      //         rateSpot %= RATE_SIZE;

      //         beatAvg = 0;
      //         for (byte x = 0 ; x < RATE_SIZE ; x++)
      //             beatAvg += rates[x];
      //         beatAvg /= RATE_SIZE;
      //     }
      // }
      // Serial.print("IR=");
      // Serial.print(irBuffer[i]);
      // Serial.print(", BPM=");
      // Serial.print(beatsPerMinute);
      // Serial.print(", Avg BPM=");
      // Serial.println(beatAvg);
      // BPM = beatAvg;
      // Serial.println(BPM);
      #ifdef __DEBUG__
        Serial.print(F(", BPM="));
        Serial.println(BPM);
      #endif
      //** BPM **//
    }

    //After gathering 25 new samples recalculate HR and SP02
    maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
  }
}
void MAX30102Controller::algorithm_2(){
  long irValue =  particleSensor.getIR();
  long redV= particleSensor.getRed();

  if (checkForBeat(irValue) == true) {
      long delta = millis() - lastBeat;
      lastBeat = millis();

      beatsPerMinute = 60 / (delta / 1000.0);

      // For Glucose
      diff= irValue-redV;                              
      A=-log((float)irValue/65536.0);
      beer= A/(2.303*0.1);

      if (beatsPerMinute < 255 && beatsPerMinute > 20) {
          rates[rateSpot++] = (byte)beatsPerMinute;
          rateSpot %= RATE_SIZE;

          beatAvg = 0;
          for (byte x = 0 ; x < RATE_SIZE ; x++)
              beatAvg += rates[x];
          beatAvg /= RATE_SIZE;
          BPM = beatAvg;
      }
      
  }



  Serial.print("IR=");
  Serial.print(irValue);
    Serial.print(",RED=");
  Serial.print(redV);
  Serial.print(", BPM=");
  Serial.print(beatsPerMinute);
  Serial.print(", Avg BPM=");
  Serial.print(beatAvg);
 Serial.print(", BGM =");
  Serial.print(beer);
Serial.print(", A =");
  Serial.print(A);

  if (irValue < 50000)
    Serial.print(" No finger?");

  Serial.println();
}

bool once = true;

void MAX30102Controller::loop(){
  Serial.println("MAX30102 Thread is working");
  if(once){
    configureMAX30100();
    once = false;
  }
  long _time2 = millis();
  while(1){
    // algorithm_1();
    algorithm_2();
  }

}
