#pragma once

#include <Arduino.h>
#include "secrets.hpp"

// LOG
#define ENABLE_LOG 1  // 1: 로그 활성화, 0: 로그 비활성화
#define LOG_SERIAL Serial

// MAIN
#define OAT_SERIAL Serial2
#define OAT_BAUD_RATE 2400