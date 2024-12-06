#ifndef D6T_H
#define D6T_H

#include <Wire.h>
#include <Arduino.h>

#define D6T_ADDR 0x0A
#define D6T_CMD 0x4C
#define N_PIXEL 1
#define N_READ ((N_PIXEL + 1) * 2 + 1)

class D6T {
public:
    D6T();
    void begin();
    bool readTemperature(double& ptat, double pix_data[]);
private:
    uint8_t calc_crc(uint8_t data);
    bool checkPEC(uint8_t buf[], int n);
};

#endif
