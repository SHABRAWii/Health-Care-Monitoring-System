#ifndef MAX30102_CUSTOM_H
#define MAX30102_CUSTOM_H

#include <Wire.h>
#include <MAX30105.h>
#include <spo2_algorithm.h>
#include <heartRate.h>


#define MAX_BRIGHTNESS 255

class MAX30102Controller {
public:
    MAX30102Controller(uint8_t _pinStatus, uint8_t _pulseLED, uint8_t _readLED, bool DEBUG);
    ~MAX30102Controller();
    void loop();
    //** START OF SPO2 **//
    int32_t spo2; //SPO2 value
    int8_t validSPO2; //indicator to show if the SPO2 calculation is valid
    int32_t heartRate; //heart rate value
    int8_t validHeartRate; //indicator to show if the heart rate calculation is valid
    //** END OF SPO2 **//
    //** START OF BPM **/
    bool status;
    int BPM;
    //** END OF BPM **/

    //** START OF GLUCOSE **/
    long A=0,beer=0,diff;
    //** END OF GLUCOSE **/


private:
    uint8_t pinStatus;
    //** START OF SPO2 **//
    uint32_t irBuffer[100]; //infrared LED sensor data
    uint32_t redBuffer[100];  //red LED sensor data
    int32_t bufferLength; //data length


    uint8_t pulseLED; //Must be on PWM pin
    uint8_t readLED; //Blinks with each data read
    void algorithm_1();
    void algorithm_2();
    //** END OF SPO2 **//

    //** START OF BPM **/
    void configureMAX30100();
    
    const byte RATE_SIZE = 4; // Increase this for more averaging. 4 is good.
    byte rates[4]; // Array of heart rates
    byte rateSpot = 0;
    long lastBeat = 0; // Time at which the last beat occurred
    float beatsPerMinute;
    int beatAvg;
    MAX30105 particleSensor;
    //** END OF BPM **/

    
};

#endif
