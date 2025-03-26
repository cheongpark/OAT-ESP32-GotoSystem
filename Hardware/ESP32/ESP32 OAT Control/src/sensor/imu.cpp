#include "sensor/imu.hpp"

namespace SENSOR {
    // ===== 싱글톤 인스턴스 가져오기 =====
    IMU& IMU::getInstance() {
        static IMU instance;
        return instance;
    }

    // ===== MPU6050 I2C 패스스루 모드 활성화 =====
    bool IMU::enableMPUBypass() {
        // MPU6050의 I2C 마스터 모드 비활성화하고 패스스루 모드 활성화
        Wire.beginTransmission(0x68);  // MPU6050 I2C 주소
        Wire.write(0x6A);              // 레지스터 주소: USER_CTRL
        Wire.write(0x00);              // I2C 마스터 모드 비활성화
        byte error = Wire.endTransmission(true);
        
        if (error != 0) {
            LOG(LOG_ERROR, "MPU6050 접근 실패");
            return false;
        }
        
        Wire.beginTransmission(0x68);  // MPU6050 I2C 주소
        Wire.write(0x37);              // 레지스터 주소: INT_PIN_CFG
        Wire.write(0x02);              // I2C 패스스루 모드 활성화
        error = Wire.endTransmission(true);
        
        if (error != 0) {
            LOG(LOG_ERROR, "MPU6050 패스스루 설정 실패");
            return false;
        }
        
        LOG(LOG_INFO, "MPU6050 I2C 패스스루 모드 활성화");
        return true;
    }

    // ===== IMU 초기화 =====
    bool IMU::init(int sda_pin, int scl_pin) {
        _sda_pin = sda_pin;
        _scl_pin = scl_pin;
        
        // I2C 초기화
        Wire.begin(sda_pin, scl_pin);
        
        // MPU6050 초기화
        if (_mpu.begin()) {
            _mpu_available = true;
            
            // 기본 설정
            _mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
            _mpu.setGyroRange(MPU6050_RANGE_500_DEG);
            _mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
            
            LOG(LOG_INFO, "MPU6050 초기화 성공");
            
            // MPU6050 I2C 패스스루 모드 활성화
            enableMPUBypass();
            delay(100); // 패스스루 모드 적용을 위한 지연
            
            // BMP180 초기화
            if (_bmp.begin()) {
                _bmp_available = true;
                LOG(LOG_INFO, "BMP180 초기화 성공");
            } else {
                LOG(LOG_WARN, "BMP180 초기화 실패 - 연결 확인 필요");
            }
            
            _initialized = true;
            return true;
        } else {
            LOG(LOG_ERROR, "IMU 초기화 실패 - MPU6050 연결 확인 필요");
            _initialized = false;
            return false;
        }
    }

    // ===== IMU 연결 상태 확인 =====
    bool IMU::isConnected() {
        return _initialized && _mpu_available;
    }

    // ===== 모든 데이터 읽기 =====
    bool IMU::read(IMUData &data) {
        if (!_initialized || !_mpu_available) {
            LOG(LOG_ERROR, "IMU가 초기화되지 않았거나 사용할 수 없습니다.");
            return false;
        }
        
        bool success = false;
        
        // MPU6050 데이터 읽기
        sensors_event_t a, g, temp;
        _mpu.getEvent(&a, &g, &temp);
        
        // 가속도 데이터 복사
        data.acceleration.x = a.acceleration.x;
        data.acceleration.y = a.acceleration.y;
        data.acceleration.z = a.acceleration.z;
        
        // 자이로스코프 데이터 복사
        data.gyroscope.x = g.gyro.x;
        data.gyroscope.y = g.gyro.y;
        data.gyroscope.z = g.gyro.z;
        
        // 온도 데이터 복사
        data.temperature.temperature = temp.temperature;
        success = true;
        
        // BMP180 데이터 읽기 (사용 가능한 경우)
        if (_bmp_available) {
            data.pressure.pressure = _bmp.readPressure();
            data.pressure.altitude = _bmp.readAltitude();
        }
        
        return success;
    }

    // ===== 개별 센서 데이터 읽기 함수 =====
    bool IMU::readAcceleration(IMUData::Acceleration &acceleration) {
        if (!_initialized || !_mpu_available) {
            LOG(LOG_ERROR, "IMU가 초기화되지 않았습니다.");
            return false;
        }
        
        sensors_event_t a, g, temp;
        _mpu.getEvent(&a, &g, &temp);
        
        acceleration.x = a.acceleration.x;
        acceleration.y = a.acceleration.y;
        acceleration.z = a.acceleration.z;
        
        return true;
    }

    bool IMU::readGyroscope(IMUData::Gyroscope &gyroscope) {
        if (!_initialized || !_mpu_available) {
            LOG(LOG_ERROR, "IMU가 초기화되지 않았습니다.");
            return false;
        }
        
        sensors_event_t a, g, temp;
        _mpu.getEvent(&a, &g, &temp);
        
        gyroscope.x = g.gyro.x;
        gyroscope.y = g.gyro.y;
        gyroscope.z = g.gyro.z;
        
        return true;
    }

    bool IMU::readTemperature(float &temperature) {
        if (!_initialized || !_mpu_available) {
            LOG(LOG_ERROR, "IMU가 초기화되지 않았습니다.");
            return false;
        }
        
        sensors_event_t a, g, temp;
        _mpu.getEvent(&a, &g, &temp);
        
        temperature = temp.temperature;
        
        return true;
    }

    bool IMU::readPressure(float &pressure) {
        if (!_initialized || !_bmp_available) {
            LOG(LOG_ERROR, "기압 센서가 초기화되지 않았습니다.");
            return false;
        }
        
        pressure = _bmp.readPressure();
        
        return true;
    }

    bool IMU::readAltitude(float &altitude) {
        if (!_initialized || !_bmp_available) {
            LOG(LOG_ERROR, "기압 센서가 초기화되지 않았습니다.");
            return false;
        }
        
        altitude = _bmp.readAltitude();
        
        return true;
    }

    // ===== 유틸리티 함수 =====
    String IMU::getAccelerationString() {
        if (!_initialized || !_mpu_available) {
            return "가속도 센서 사용 불가";
        }
        
        IMUData::Acceleration acc;
        if (readAcceleration(acc)) {
            char buffer[50];
            sprintf(buffer, "X: %.2f, Y: %.2f, Z: %.2f m/s²", acc.x, acc.y, acc.z);
            return String(buffer);
        } else {
            return "가속도 데이터 읽기 실패";
        }
    }

    String IMU::getGyroscopeString() {
        if (!_initialized || !_mpu_available) {
            return "자이로스코프 센서 사용 불가";
        }
        
        IMUData::Gyroscope gyro;
        if (readGyroscope(gyro)) {
            char buffer[50];
            sprintf(buffer, "X: %.2f, Y: %.2f, Z: %.2f rad/s", gyro.x, gyro.y, gyro.z);
            return String(buffer);
        } else {
            return "자이로스코프 데이터 읽기 실패";
        }
    }

    String IMU::getTemperatureString() {
        if (!_initialized || !_mpu_available) {
            return "온도 센서 사용 불가";
        }
        
        float temp;
        if (readTemperature(temp)) {
            char buffer[20];
            sprintf(buffer, "%.1f °C", temp);
            return String(buffer);
        } else {
            return "온도 데이터 읽기 실패";
        }
    }

    String IMU::getPressureString() {
        if (!_initialized || !_bmp_available) {
            return "기압 센서 사용 불가";
        }
        
        float pressure;
        if (readPressure(pressure)) {
            char buffer[20];
            sprintf(buffer, "%.2f Pa", pressure);
            return String(buffer);
        } else {
            return "기압 데이터 읽기 실패";
        }
    }

    String IMU::getAltitudeString() {
        if (!_initialized || !_bmp_available) {
            return "고도 센서 사용 불가";
        }
        
        float altitude;
        if (readAltitude(altitude)) {
            char buffer[20];
            sprintf(buffer, "%.2f m", altitude);
            return String(buffer);
        } else {
            return "고도 데이터 읽기 실패";
        }
    }
}