#pragma once

#include <Arduino.h>
#include <TinyGPSPlus.h>
#include <time.h>

#include "utils.hpp"
#include "log.hpp"
#include "settings.hpp"

// GPS 쪽은 전부 Claude가 작성함 (나중에 수정이나 필요한 기능만 남겨두고 최적화 필요)

// GPS 센서 관련 처리 클래스 (싱글톤 패턴 적용)
namespace SENSOR {
    class GPS {
    private:
        // ===== 시리얼 및 상태 관련 변수 =====
#if !GPS_FAKE_DATA
        HardwareSerial* _gps_serial;
        TinyGPSPlus _gps;
#endif
        unsigned long _baud_rate;
        bool _initialized;
        bool _connected;
        TaskHandle_t _gps_task_handle;
        
        // ===== 위치 데이터 =====
        double _latitude;
        double _longitude;
        double _altitude;
        int _satellites;
        float _hdop;
        
        // ===== 시간 데이터 =====
        // UTC 기준 시간
        int _utc_year, _utc_month, _utc_day;
        int _utc_hour, _utc_minute, _utc_second;
        
        // 로컬 시간 (타임존 적용)
        int _local_year, _local_month, _local_day;
        int _local_hour, _local_minute, _local_second;
        
        // ===== 기본값 =====
        double _default_latitude;
        double _default_longitude;
        double _default_altitude;
        int _default_year, _default_month, _default_day;
        int _default_hour, _default_minute, _default_second;
        
        // ===== 내부 타이머 =====
        unsigned long _last_update_time;
        unsigned long _last_gps_process_time;
        
        // ===== 내부 처리 함수 =====
        // 생성자 - 싱글톤 패턴을 위해 private으로 선언
        GPS();
        
        // FreeRTOS 태스크 처리 함수
        static void gpsTask(void* parameter);
        
#if GPS_FAKE_DATA
        // 가짜 GPS 데이터 생성
        void generateFakeData();
#else
        // 실제 GPS 시리얼 데이터 처리
        void processGPSData();
#endif
        
        // 데이터 유효성 검사 및 업데이트
        void validateData();
        
        // UTC 시간을 로컬 시간으로 변환
        void convertUTCtoLocal();
        
        // 시간 업데이트 함수
        void updateCurrentTime(int &hour, int &minute, int &second);
        void updateCurrentDateTime(int &year, int &month, int &day, int &hour, int &minute, int &second);
        
        // 시스템 시간 설정 헬퍼 함수
        void setDefaultSystemTime();

    public:
        // ===== 싱글톤 패턴 구현 =====
        GPS(const GPS&) = delete;
        GPS& operator=(const GPS&) = delete;
        static GPS& getInstance();

        // ===== 초기화 및 연결 관련 함수 =====
        void init(HardwareSerial &gps_serial, const unsigned long baud_rate, const int rxPin, const int txPin);
        bool connect();
        bool isConnected();
        void disconnect();
        
        // ===== 위치 데이터 접근 함수 =====
        double getLatitude();
        double getLongitude();
        double getAltitude();
        int getSatellites();
        float getHDOP();
        void getLocation(double &lat, double &lng, double &alt);
        String getFormattedLocation();
        
        // ===== UTC 시간 관련 함수 =====
        void getUTCDate(int &year, int &month, int &day);
        void getUTCTime(int &hour, int &minute, int &second);
        String getFormattedUTCDate();
        String getFormattedUTCTime();
        String getFormattedUTCDateTime();
        
        // ===== 로컬 시간 관련 함수 =====
        void getLocalDate(int &year, int &month, int &day);
        void getLocalTime(int &hour, int &minute, int &second);
        String getFormattedLocalDate();
        String getFormattedLocalTime();
        String getFormattedLocalDateTime();
        
        // ===== 이전 버전 호환용 함수 (기본적으로 로컬 시간 반환) =====
        void getDate(int &year, int &month, int &day);
        void getTime(int &hour, int &minute, int &second);
        String getFormattedDate();
        String getFormattedTime();
        
        // ===== 상태 확인 함수 =====
        bool hasValidLocation();
        bool hasValidTime();
        bool hasValidAltitude();
        
        // ===== 기본값 설정 함수 =====
        void setDefaultLocation(double latitude, double longitude, double altitude);
        void setDefaultDate(int year, int month, int day);
        void setDefaultTime(int hour, int minute, int second);
        void setDefaultDateTime(int year, int month, int day, int hour, int minute, int second);
        
        // ===== 기본값 조회 함수 =====
        void getDefaultLocation(double &latitude, double &longitude, double &altitude);
        void getDefaultDate(int &year, int &month, int &day);
        void getDefaultTime(int &hour, int &minute, int &second);
        
        // ===== 수동 설정 함수 =====
        void setCurrentLocation(double latitude, double longitude, double altitude);
        void setCurrentUTCTime(int year, int month, int day, int hour, int minute, int second);
    };
} 