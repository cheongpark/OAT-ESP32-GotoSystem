// log.hpp
#pragma once

#include <Arduino.h>

#include "settings.hpp"

#if LOG_ENABLE == 1

// 로그 레벨 정의
enum LogLevel {
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR
};

// 내부 함수 정의
inline void log_message(LogLevel level, const char* file, int line, const char* func, const String message) {
    String level_str;
    
    switch (level) {
        case LOG_INFO:  level_str = "INFO"; break;
        case LOG_WARN:  level_str = "WARN"; break;
        case LOG_ERROR: level_str = "ERROR"; break;
    }

    char buffer[256];
    sprintf(buffer, "[%s] %s:%d (%s) - %s", level_str, file, line, func, message.c_str());
    LOG_SERIAL.println(buffer);
}

#define LOG(level, message) log_message(level, __FILE__, __LINE__, __func__, message)
#else

// LOG_ENABLE가 정의되지 않으면 빈 매크로로 처리
#define LOG(level, message) ((void)0)

#endif