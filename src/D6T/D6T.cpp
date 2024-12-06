#include "D6T.h"

D6T::D6T() {}

void D6T::begin() {
    Wire.begin();
    delay(220);
}

bool D6T::readTemperature(double& ptat, double pix_data[]) {
    uint8_t rbuf[N_READ];
    memset(rbuf, 0, N_READ);
    Wire.beginTransmission(D6T_ADDR);
    Wire.write(D6T_CMD);
    Wire.endTransmission();
    Wire.requestFrom(D6T_ADDR, N_READ);
    int i = 0;
    while (Wire.available()) {
        rbuf[i++] = Wire.read();
    }
    if (!checkPEC(rbuf, N_READ - 1)) {
        return false;
    }
    ptat = (double)((int16_t)rbuf[0] + ((int16_t)rbuf[1] << 8)) / 10.0;
    for (i = 0; i < N_PIXEL; i++) {
        int16_t itemp = (int16_t)rbuf[2 + 2 * i] + ((int16_t)rbuf[3 + 2 * i] << 8);
        pix_data[i] = (double)itemp / 10.0;
    }
    return true;
}

uint8_t D6T::calc_crc(uint8_t data) {
    uint8_t temp;
    for (int index = 0; index < 8; index++) {
        temp = data;
        data <<= 1;
        if (temp & 0x80) {
            data ^= 0x07;
        }
    }
    return data;
}

bool D6T::checkPEC(uint8_t buf[], int n) {
    uint8_t crc = calc_crc((D6T_ADDR << 1) | 1);
    for (int i = 0; i < n; i++) {
        crc = calc_crc(buf[i] ^ crc);
    }
    if (crc != buf[n]) {
        Serial.print("PEC check failed:");
        Serial.print(crc, HEX);
        Serial.print("(cal) vs ");
        Serial.print(buf[n], HEX);
        Serial.println("(get)");
        return false;
    }
    return true;
}
