#pragma once

#include <Arduino.h>
#include <unity.h>

class HallSensor {
public:
    HallSensor();
    
    void init(int pin);
    bool read(bool &data);

private:
    int pin;
};

HallSensor::HallSensor() {
    
}

void HallSensor::init(int pin) {
    this->pin = pin;
    pinMode(pin, INPUT);
}

bool HallSensor::read(bool &data) {
    data = digitalRead(pin);
    
    return true;
}