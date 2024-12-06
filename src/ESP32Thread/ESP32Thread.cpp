#include "ESP32Thread.h"

ESP32Thread::ESP32Thread(uint16_t _size) : taskHandle(NULL), size(_size) {}

ESP32Thread::~ESP32Thread() {
    if (taskHandle != NULL) {
        vTaskDelete(taskHandle);
        taskHandle = NULL;
    }
}

void ESP32Thread::begin(std::function<void()> taskFunction) {
    xTaskCreate(
        taskWrapper,          // Task function
        "ESP32ThreadTask",    // Task name
        10000,                 // Stack size (words)
        new std::function<void()>(taskFunction), // Parameters
        2,                    // Task priority
        &taskHandle           // Task handle
    );
}
void ESP32Thread::end() {
    if (taskHandle != NULL) {
        vTaskDelete(taskHandle);
        taskHandle = NULL;
    }
}

void ESP32Thread::delay(unsigned long ms) {
    vTaskDelay(pdMS_TO_TICKS(ms));
}

void ESP32Thread::yield() {
    taskYIELD();
}

void ESP32Thread::taskWrapper(void* taskPtr) {
    std::function<void()>* taskFunction = reinterpret_cast<std::function<void()>*>(taskPtr);
    (*taskFunction)();
    delete taskFunction;
    vTaskDelete(NULL);
}
