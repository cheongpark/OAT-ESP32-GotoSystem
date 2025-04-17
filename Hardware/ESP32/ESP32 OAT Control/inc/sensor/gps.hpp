#pragma once

#include <Arduino.h>
#include <TinyGPSPlus.h>
#include <time.h>

#include "utils.hpp"
#include "log.hpp"
#include "settings.hpp"

// 타임존 설정 구조체
struct TimeZone {
    const char* name;  // 타임존 이름 (예: "KST", "UTC", "EST" 등)
    int offset;        // UTC 기준 시차 (시간)
};

// 기본 타임존 설정
const TimeZone DEFAULT_TIMEZONE = {"UTC", 0};  // 기본값은 UTC

// 일반적인 타임존 목록
const TimeZone TIMEZONES[] = {
    {"UTC", 0},      // UTC (협정 세계시)
    {"KST", 9},      // 한국 표준시
    {"JST", 9},      // 일본 표준시
    {"EST", -5},     // 미국 동부 표준시
    {"CST", -6},     // 미국 중부 표준시
    {"PST", -8},     // 미국 태평양 표준시
    {"GMT", 0},      // 그리니치 표준시
    {"CET", 1},      // 중앙 유럽 표준시
    {"EET", 2},      // 동부 유럽 표준시
    {"AEST", 10}     // 호주 동부 표준시
};

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
        
        // ===== 타임존 관련 변수 =====
        TimeZone _current_timezone;  // 현재 설정된 타임존
        
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
        static void gpsTask(void* parameter); // GPS 데이터 처리 태스크
        
#if GPS_FAKE_DATA
        // 가짜 GPS 데이터 생성
        void generateFakeData(); // 테스트용 가짜 GPS 데이터 생성
#else
        // 실제 GPS 시리얼 데이터 처리
        void processGPSData(); // GPS 시리얼 데이터 처리 및 파싱
#endif
        
        // 데이터 유효성 검사 및 업데이트
        void validateData(); // GPS 데이터 유효성 검사 및 업데이트
        
        // UTC 시간을 로컬 시간으로 변환
        void convertUTCtoLocal(); // UTC 시간을 로컬 시간으로 변환
        
        // 시간 업데이트 함수
        void updateCurrentTime(int &hour, int &minute, int &second); // 현재 시간 업데이트
        void updateCurrentDateTime(int &year, int &month, int &day, int &hour, int &minute, int &second); // 현재 날짜와 시간 업데이트
        
        // 시스템 시간 설정 헬퍼 함수
        void setDefaultSystemTime(); // 시스템 기본 시간 설정

    public:
        // ===== 싱글톤 패턴 구현 =====
        GPS(const GPS&) = delete;
        GPS& operator=(const GPS&) = delete;
        static GPS& getInstance();

        // ===== 초기화 및 연결 관련 함수 =====
        void init(HardwareSerial &gps_serial, const unsigned long baud_rate, const int rxPin, const int txPin); // GPS 하드웨어 초기화 및 설정
        bool connect(); // GPS 연결 시도 및 결과 반환
        bool isConnected(); // 현재 GPS 연결 상태 확인
        void disconnect(); // GPS 연결 해제
        
        // ===== 위치 데이터 접근 함수 =====
        double getLatitude(); // 현재 위도 값 반환
        double getLongitude(); // 현재 경도 값 반환
        double getAltitude(); // 현재 고도 값 반환
        int getSatellites(); // 현재 추적 중인 위성 수 반환
        float getHDOP(); // GPS 수평 정확도 지표 반환
        void getLocation(double &lat, double &lng, double &alt); // 현재 위치 정보를 참조로 반환
        String getFormattedLocation(); // 포맷된 위치 정보 문자열 반환
        
        // ===== UTC 시간 관련 함수 =====
        void getUTCDate(int &year, int &month, int &day); // UTC 날짜 정보를 참조로 반환
        void getUTCTime(int &hour, int &minute, int &second); // UTC 시간 정보를 참조로 반환
        String getFormattedUTCDate(); // 포맷된 UTC 날짜 문자열 반환
        String getFormattedUTCTime(); // 포맷된 UTC 시간 문자열 반환
        String getFormattedUTCDateTime(); // 포맷된 UTC 날짜와 시간 문자열 반환
        
        // ===== 로컬 시간 관련 함수 =====
        void getLocalDate(int &year, int &month, int &day); // 로컬 날짜 정보를 참조로 반환
        void getLocalTime(int &hour, int &minute, int &second); // 로컬 시간 정보를 참조로 반환
        String getFormattedLocalDate(); // 포맷된 로컬 날짜 문자열 반환
        String getFormattedLocalTime(); // 포맷된 로컬 시간 문자열 반환
        String getFormattedLocalDateTime(); // 포맷된 로컬 날짜와 시간 문자열 반환
        
        // ===== 이전 버전 호환용 함수 (기본적으로 로컬 시간 반환) =====
        void getDate(int &year, int &month, int &day); // 로컬 날짜 정보를 참조로 반환 (이전 버전 호환)
        void getTime(int &hour, int &minute, int &second); // 로컬 시간 정보를 참조로 반환 (이전 버전 호환)
        String getFormattedDate(); // 포맷된 로컬 날짜 문자열 반환 (이전 버전 호환)
        String getFormattedTime(); // 포맷된 로컬 시간 문자열 반환 (이전 버전 호환)
        
        // ===== 기본값 설정 함수 =====
        void setDefaultLocation(double latitude, double longitude, double altitude); // 기본 위치 정보 설정
        void setDefaultDate(int year, int month, int day); // 기본 날짜 설정
        void setDefaultTime(int hour, int minute, int second); // 기본 시간 설정
        
        // ===== 기본값 조회 함수 =====
        void getDefaultLocation(double &latitude, double &longitude, double &altitude); // 기본 위치 정보를 참조로 반환
        void getDefaultDate(int &year, int &month, int &day); // 기본 날짜 정보를 참조로 반환
        void getDefaultTime(int &hour, int &minute, int &second); // 기본 시간 정보를 참조로 반환
        
        // ===== 수동 설정 함수 =====
        void setCurrentLocation(double latitude, double longitude, double altitude); // 현재 위치 정보 수동 설정
        void setCurrentUTCTime(int year, int month, int day, int hour, int minute, int second); // 현재 UTC 시간 정보 수동 설정

        // ===== 타임존 관련 함수 =====
        void setTimezone(const char* timezone_name); // 타임존 설정
        void setTimezone(const TimeZone& timezone);  // 타임존 설정 (구조체 직접 전달)
        const TimeZone& getCurrentTimezone() const;  // 현재 타임존 정보 반환
        int getTimezoneOffset() const;              // 현재 타임존의 UTC 시차 반환
        const char* getTimezoneName() const;        // 현재 타임존 이름 반환
    };
}