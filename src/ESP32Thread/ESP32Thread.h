#ifndef ESP32_THREAD_H
#define ESP32_THREAD_H

#include <Arduino.h>
#include <functional>

class ESP32Thread {
  public:
    ESP32Thread(uint16_t _size);
    ~ESP32Thread();

    void begin(std::function<void()> taskFunction);
    void end();
    void delay(unsigned long ms);
    void yield();

  private:
    uint16_t size;
    TaskHandle_t taskHandle;
    static void taskWrapper(void* taskPtr);
};

#endif
