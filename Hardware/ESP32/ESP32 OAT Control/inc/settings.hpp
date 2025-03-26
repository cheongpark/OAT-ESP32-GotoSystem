#pragma once

#include <Arduino.h>
#include "secrets.hpp"

// LOG
#define ENABLE_LOG 1  // 1: 로그 활성화, 0: 로그 비활성화
#define LOG_SERIAL Serial

// MAIN
#define OAT_SERIAL Serial2 // (16, 17) pin
#define OAT_BAUD_RATE 19200 

// WiFi
#define WIFI_IP "192.168.4.1"
#define WIFI_GATEWAY "192.168.4.1"
#define WIFI_SUBNET "255.255.255.0"

// OAT
extern float RADEC_SLEW_RATES[6]; // 0, 1, 2, 3, 4, 5
extern float AZALT_SLEW_RATES[6]; // 0, 1, 2, 3, 4, 5
