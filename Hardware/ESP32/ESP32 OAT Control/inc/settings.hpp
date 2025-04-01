#pragma once

#include <Arduino.h>
#include "secrets.hpp"

// LOG
#define LOG_ENABLE 1  // 1: 로그 활성화, 0: 로그 비활성화
#define LOG_SERIAL Serial

// OAT
#define OAT_SERIAL Serial2 // (16, 17) pin
#define OAT_BAUD_RATE 19200
#define OAT_SERIAL_RX 17
#define OAT_SERIAL_TX 16

extern float OAT_RADEC_SLEW_RATES[6]; // extern 무조건 필요(전역변수), 0, 1, 2, 3, 4, 5 
extern float OAT_AZALT_SLEW_RATES[6]; // extern 무조건 필요(전역변수), 0, 1, 2, 3, 4, 5

// WiFi
#define WIFI_IP "192.168.4.1"
#define WIFI_GATEWAY "192.168.4.1"
#define WIFI_SUBNET "255.255.255.0"
#define WIFI_MDNS_HOSTNAME "oat"  // mDNS 호스트네임 (oat.local로 접속 가능)

// GPS
#define GPS_SERIAL Serial1 // (23, 19) pin
#define GPS_BAUD_RATE 9600
#define GPS_SERIAL_RX 19
#define GPS_SERIAL_TX 23
#define GPS_FAKE_DATA 0 // 1: 가짜 데이터 사용, 0: 실제 데이터 사용

// GPS 기본 위치 및 시간 설정 (서울 위치)
#define GPS_DEFAULT_LATITUDE 37.5665
#define GPS_DEFAULT_LONGITUDE 126.9780
#define GPS_DEFAULT_ALTITUDE 38.0 // 서울 평균 고도 (미터)
#define GPS_DATA_TIMEOUT_MS 1800000 // 30분 (밀리초 단위)
#define GPS_UPDATE_INTERVAL_MS 1000 // GPS 데이터 업데이트 간격 (1초)
#define GPS_TIMEZONE_OFFSET 9 // 시간대 오프셋 (UTC+9: 한국 표준시)

// IMU
#define IMU_I2C_SDA 21
#define IMU_I2C_SCL 22

// Data
#define DATA_UPDATE_INTERVAL_MS 1000 // 1초