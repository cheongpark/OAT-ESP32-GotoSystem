#include <Arduino.h>
#include <unity.h>

class HallSensor {
public:
    HallSensor(int pin);
    
    bool read(bool &data);

private:
    int pin;
};

HallSensor::HallSensor(int pin) {
    this->pin = pin;
}

bool HallSensor::read(bool &data) {
    data = digitalRead(pin);

    return true;
}