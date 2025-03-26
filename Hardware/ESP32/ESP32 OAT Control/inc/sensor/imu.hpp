#pragma once

#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>
#include <Wire.h>

#include "utils.hpp"
#include "log.hpp"

// IMU 쪽은 전부 Claude가 작성함 (나중에 수정이나 필요한 기능만 남겨두고 최적화 필요)

// IMU 관련 처리 클래스 (I2C)
namespace SENSOR {
    // IMU 데이터 구조체
    struct IMUData {
        // 가속도 데이터
        struct Acceleration {
            float x;
            float y;
            float z;
        } acceleration;

        // 자이로스코프 데이터
        struct Gyroscope {
            float x;
            float y;
            float z;
        } gyroscope;

        // 온도 데이터
        struct Temperature {
            float temperature;
        } temperature;
        
        // 기압/고도 데이터
        struct Pressure {
            float pressure;
            float altitude;
        } pressure;
    };

    class IMU {
    private:
        // ===== 센서 객체 =====
        Adafruit_MPU6050 _mpu;
        Adafruit_BMP085 _bmp;
        
        // ===== 상태 변수 =====
        bool _initialized;
        bool _mpu_available;
        bool _bmp_available;
        int _sda_pin;
        int _scl_pin;
        
        // ===== 싱글톤 패턴을 위한 비공개 생성자 =====
        IMU() : 
            _initialized(false),
            _mpu_available(false),
            _bmp_available(false),
            _sda_pin(0),
            _scl_pin(0) {}
        
        // ===== 내부 초기화 함수 =====
        bool enableMPUBypass();

    public:
        // ===== 싱글톤 패턴 구현 =====
        IMU(const IMU&) = delete;
        IMU& operator=(const IMU&) = delete;
        static IMU& getInstance();

        // ===== 초기화 및 연결 관련 함수 =====
        bool init(int sda_pin, int scl_pin);
        bool isConnected();
        bool read(IMUData &data);
        
        // ===== 개별 센서 데이터 읽기 함수 =====
        bool readAcceleration(IMUData::Acceleration &acceleration);
        bool readGyroscope(IMUData::Gyroscope &gyroscope);
        bool readTemperature(float &temperature);
        bool readPressure(float &pressure);
        bool readAltitude(float &altitude);
        
        // ===== 유틸리티 함수 =====
        String getAccelerationString();
        String getGyroscopeString();
        String getTemperatureString();
        String getPressureString();
        String getAltitudeString();
    };
}