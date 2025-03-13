#pragma once

#include <Arduino.h>

#include "log.hpp"

// OAT와 시리얼 통신으로 데이터를 주고 받고 하는 것들
// 싱글톤 패턴으로 구현

class MountSerial {
private:
    HardwareSerial* mnt_serial;
    unsigned long baud_rate;
    bool initialized;
    String product_name;
    String firmware_version;

    MountSerial() : mnt_serial(nullptr), baud_rate(0), initialized(false) {}
    ~MountSerial() {}

public:
    static MountSerial& getInstance() {
        static MountSerial instance;
        return instance;
    }

    MountSerial(const MountSerial&) = delete;
    MountSerial& operator=(const MountSerial&) = delete;

    void init(HardwareSerial &mnt_serial, unsigned long baud_rate);
    bool connect();
    bool isConnected();
    
    String request(const String command);
    void getInfo();
    
    // Getters
    String getProductName() const { return product_name; }
    String getFirmwareVersion() const { return firmware_version; }
};