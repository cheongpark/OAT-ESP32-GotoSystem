#pragma once

#include <Arduino.h>
#include <unity.h>
#include <TinyGPSPlus.h>

// GPS 데이터 구조체
struct GPSData {
    float latitude;         // 위도
    float longitude;        // 경도
    float altitude;         // 고도 (m)
    float speed;            // 속도 (km/h)
    float course;           // 방향 (도)
    int satellites;         // 위성 수
    float hdop;             // 수평정밀도 (값이 낮을수록 정확도가 높음)
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    bool locationValid;     // 위치 데이터 유효 여부
    bool timeValid;         // 시간 데이터 유효 여부

    // 텔레플롯에 데이터 출력
    void printTelePlot(const char* prefix = "GPS_") {
        // 위성 정보는 항상 출력
        Serial.print(">");
        Serial.print(prefix);
        Serial.print("Satellites:");
        Serial.println(satellites);
        
        Serial.print(">");
        Serial.print(prefix);
        Serial.print("HDOP:");
        Serial.println(hdop);
        
        // 위치가 유효한 경우만 위치 관련 데이터 출력
        if (locationValid) {
            Serial.print(">");
            Serial.print(prefix);
            Serial.print("Lat:");
            Serial.println(latitude, 6);
            
            Serial.print(">");
            Serial.print(prefix);
            Serial.print("Lng:");
            Serial.println(longitude, 6);
            
            Serial.print(">");
            Serial.print(prefix);
            Serial.print("Alt:");
            Serial.println(altitude);
            
            Serial.print(">");
            Serial.print(prefix);
            Serial.print("Speed:");
            Serial.println(speed);
            
            Serial.print(">");
            Serial.print(prefix);
            Serial.print("Course:");
            Serial.println(course);
        }
    }
    
    // 기본 정보 출력
    void printInfo() {
        // 감지된 위성 정보는 항상 표시
        Serial.print("감지된 위성 수: ");
        Serial.println(satellites);
        
        Serial.print("HDOP(수평정밀도): ");
        Serial.println(hdop, 2);
        Serial.println("(낮을수록 정밀도가 높음, 1.0 이하는 좋은 상태)");
        
        if (locationValid) {
            Serial.print("위치: ");
            Serial.print(latitude, 6);
            Serial.print(", ");
            Serial.println(longitude, 6);
            Serial.print("고도: ");
            Serial.print(altitude);
            Serial.println("m");
            Serial.print("속도: ");
            Serial.print(speed);
            Serial.println("km/h");
            Serial.print("방향: ");
            Serial.print(course);
            Serial.println("°");
        } else {
            Serial.println("위치 정보가 유효하지 않습니다.");
            if (satellites > 0) {
                if (satellites < 3) {
                    Serial.println("위치 확정에는 최소 3개 이상의 위성이 필요합니다.");
                } else {
                    Serial.println("충분한 위성이 감지되었으나 위치가 아직 확정되지 않았습니다.");
                    Serial.println("신호 품질이 좋지 않거나 위성 배치가 적절하지 않을 수 있습니다.");
                }
            }
        }
        
        if (timeValid) {
            Serial.print("시간: ");
            Serial.print(year);
            Serial.print("-");
            if (month < 10) Serial.print("0");
            Serial.print(month);
            Serial.print("-");
            if (day < 10) Serial.print("0");
            Serial.print(day);
            Serial.print(" ");
            if (hour < 10) Serial.print("0");
            Serial.print(hour);
            Serial.print(":");
            if (minute < 10) Serial.print("0");
            Serial.print(minute);
            Serial.print(":");
            if (second < 10) Serial.print("0");
            Serial.println(second);
        } else {
            Serial.println("시간 정보가 유효하지 않습니다.");
        }
    }
};

class GPSSensor {
public:
    GPSSensor();
    
    void init(HardwareSerial &gps_serial, int baudRate = 9600);
    bool update();
    bool read(GPSData &data);
    bool read(uint8_t &data);
    
    // 최근 수신된 NMEA 문장들을 가져오는 메소드 추가
    String* getRecentNmeaSentences(int &count) {
        count = nmeaSentenceCount;
        return recentNmeaSentences;
    }

private:
    HardwareSerial *gps_serial;
    TinyGPSPlus gps;
    unsigned long lastUpdateTime;
    GPSData currentData;
    
    // 최근 NMEA 문장을 저장하기 위한 배열
    String recentNmeaSentences[10];
    int nmeaSentenceCount = 0;
    
    void updateGPSData();
};

GPSSensor::GPSSensor() {
    lastUpdateTime = 0;
}

void GPSSensor::init(HardwareSerial &gps_serial, int baudRate) {
    this->gps_serial = &gps_serial;
    
    // ESP32에서 Serial1/Serial2 핀 설정
    #if defined(ESP32)
    if (&gps_serial == &Serial1) {
        Serial.print("ESP32 Serial1 사용 중 - RX:9, TX:10, 통신 속도: ");
        Serial.println(baudRate);
        gps_serial.begin(baudRate, SERIAL_8N1, 9, 10); // RX: GPIO9, TX: GPIO10
    } else if (&gps_serial == &Serial2) {
        Serial.print("ESP32 Serial2 사용 중 - RX:16, TX:17, 통신 속도: ");
        Serial.println(baudRate);
        gps_serial.begin(baudRate, SERIAL_8N1, 16, 17); // RX: GPIO16, TX: GPIO17
    } else {
        Serial.print("일반 시리얼 포트 사용 중, 통신 속도: ");
        Serial.println(baudRate);
        gps_serial.begin(baudRate); 
    }
    #else
    Serial.print("일반 시리얼 포트 사용 중, 통신 속도: ");
    Serial.println(baudRate);
    gps_serial.begin(baudRate);
    #endif
    
    Serial.println("GPS 센서 초기화됨 (Neo M8N)");
}

bool GPSSensor::update() {
    bool newData = false;
    static String nmeaLine = "";  // NMEA 문장을 모았다가 한 줄씩 출력하기 위한 변수
    
    // GPS 데이터 수신 및 파싱
    while (gps_serial->available()) {
        char c = gps_serial->read();
        
        // NMEA 문장 수집
        if (c == '\n') {
            // 한 줄이 완성되면 출력 및 저장
            if (nmeaLine.length() > 0) {
                // Serial.print 제거 - 원시 데이터는 SensorTest에서 처리
                
                // 완성된 NMEA 문장 저장 (최대 10개까지)
                if (nmeaLine.startsWith("$")) {
                    // 배열 순환 - 가장 오래된 항목을 새 항목으로 대체
                    int idx = nmeaSentenceCount % 10;
                    recentNmeaSentences[idx] = nmeaLine;
                    
                    if (nmeaSentenceCount < 10) {
                        nmeaSentenceCount++;
                    }
                }
                
                nmeaLine = "";
            }
        } 
        else if (c == '\r') {
            // 캐리지 리턴 무시
        }
        else if (c >= 32 && c <= 126) {
            // 출력 가능한 ASCII 문자만 수집
            nmeaLine += c;
        }
        
        // TinyGPS++ 라이브러리로 파싱
        if (gps.encode(c)) {
            newData = true;
        }
    }
    
    // 새 데이터가 있으면 GPS 데이터 업데이트
    if (newData) {
        updateGPSData();
    }
    
    return newData;
}

void GPSSensor::updateGPSData() {
    // 위치 정보 업데이트
    if (gps.location.isUpdated()) {
        currentData.latitude = gps.location.lat();
        currentData.longitude = gps.location.lng();
        currentData.locationValid = gps.location.isValid();
    }
    
    // 고도 정보 업데이트
    if (gps.altitude.isUpdated()) {
        currentData.altitude = gps.altitude.meters();
    }
    
    // 속도 정보 업데이트
    if (gps.speed.isUpdated()) {
        currentData.speed = gps.speed.kmph();
    }
    
    // 방향 정보 업데이트
    if (gps.course.isUpdated()) {
        currentData.course = gps.course.deg();
    }
    
    // 시간 정보 업데이트
    if (gps.time.isUpdated() && gps.date.isUpdated()) {
        currentData.year = gps.date.year();
        currentData.month = gps.date.month();
        currentData.day = gps.date.day();
        currentData.hour = gps.time.hour();
        currentData.minute = gps.time.minute();
        currentData.second = gps.time.second();
        currentData.timeValid = gps.time.isValid() && gps.date.isValid();
    }
    
    // 위성 정보 업데이트
    currentData.satellites = gps.satellites.value();
    currentData.hdop = gps.hdop.hdop(); // 수평정밀도 (값이 낮을수록 정확도가 높음)
    
    lastUpdateTime = millis();
}

bool GPSSensor::read(GPSData &data) {
    // 데이터 업데이트
    bool updated = update();
    
    // 현재 데이터 복사
    data = currentData;
    
    return updated || (millis() - lastUpdateTime < 2000); // 2초 이내 업데이트된 데이터는 유효하다고 판단
}

bool GPSSensor::read(uint8_t &data) {
    if(gps_serial->available()) {
        data = gps_serial->read();
        return true;
    }
    return false;
}