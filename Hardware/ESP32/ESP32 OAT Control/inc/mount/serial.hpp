#pragma once

#include <Arduino.h>

#include "log.hpp"
#include "utils.hpp"

// OAT와 시리얼 통신으로 데이터를 주고 받고 하는 것들
// 싱글톤 패턴으로 구현

namespace Mount {
    class Serial {
    private:
        HardwareSerial* mnt_serial;
        unsigned long baud_rate;
        bool initialized;
        
        Serial() : mnt_serial(nullptr), baud_rate(0), initialized(false) {}

    public:
        // Singletone
        Serial(const Serial&) = delete;
        Serial& operator=(const Serial&) = delete;

        static Serial& getInstance();

        // Fucn
        void init(HardwareSerial &mnt_serial, const unsigned long baud_rate, const int rxPin, const int txPin);
        bool connect();
        bool isConnected();
        
        Response request(const String command, bool is_response = true);
    };
}

