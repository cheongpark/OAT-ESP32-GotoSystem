#include <Arduino.h>
#include <unity.h>
#include <string>

class GPSSensor {
public:
    GPSSensor();
    
    void init(HardwareSerial &gps_serial);
    bool read(uint8_t &data);

private:
    HardwareSerial *gps_serial;
};

GPSSensor::GPSSensor() {

}

void GPSSensor::init(HardwareSerial &gps_serial) {
    this->gps_serial = &gps_serial;
}

bool GPSSensor::read(uint8_t &data) {
    if(gps_serial->available()) {
        data = gps_serial->read();
        return true;
    }
    return false;
}