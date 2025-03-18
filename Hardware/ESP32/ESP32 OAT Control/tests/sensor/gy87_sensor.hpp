#pragma once

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <Adafruit_BMP085.h>
#include <Wire.h>

class GY87Data {
public:
  struct Acceleration {
    float x;
    float y;
    float z;
  } acceleration;

  struct Gyroscope {
    float x;
    float y;
    float z;
  } gyroscope;

  struct Temperature {
    float temperature;
  } temperature;
  
  struct Pressure {
    float pressure;
    float altitude;
  } pressure;
  
  struct Magnetic {
    float x;
    float y;
    float z;
    float heading;
  } magnetic;

  // 텔레플롯에 데이터 출력
  void printTelePlot(const char* prefix = "GY87_") {
    // 가속도계 데이터
    Serial.print(">");
    Serial.print(prefix);
    Serial.print("AccX:");
    Serial.println(acceleration.x);
    
    Serial.print(">");
    Serial.print(prefix);
    Serial.print("AccY:");
    Serial.println(acceleration.y);
    
    Serial.print(">");
    Serial.print(prefix);
    Serial.print("AccZ:");
    Serial.println(acceleration.z);
    
    // 자이로스코프 데이터
    Serial.print(">");
    Serial.print(prefix);
    Serial.print("GyroX:");
    Serial.println(gyroscope.x);
    
    Serial.print(">");
    Serial.print(prefix);
    Serial.print("GyroY:");
    Serial.println(gyroscope.y);
    
    Serial.print(">");
    Serial.print(prefix);
    Serial.print("GyroZ:");
    Serial.println(gyroscope.z);
    
    // 온도 데이터
    Serial.print(">");
    Serial.print(prefix);
    Serial.print("Temp:");
    Serial.println(temperature.temperature);
    
    // 지자기 센서 데이터
    Serial.print(">");
    Serial.print(prefix);
    Serial.print("MagX:");
    Serial.println(magnetic.x);
    
    Serial.print(">");
    Serial.print(prefix);
    Serial.print("MagY:");
    Serial.println(magnetic.y);
    
    Serial.print(">");
    Serial.print(prefix);
    Serial.print("MagZ:");
    Serial.println(magnetic.z);
    
    Serial.print(">");
    Serial.print(prefix);
    Serial.print("Heading:");
    Serial.println(magnetic.heading);
    
    // 기압/고도 데이터
    Serial.print(">");
    Serial.print(prefix);
    Serial.print("Pressure:");
    Serial.println(pressure.pressure);
    
    Serial.print(">");
    Serial.print(prefix);
    Serial.print("Altitude:");
    Serial.println(pressure.altitude);
  }
};

class GY87Sensor {
public:
  GY87Sensor();
  
  bool init();
  bool read(GY87Data &data);
  bool enableMPUBypass();

private:
  Adafruit_MPU6050 mpu;
  Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
  Adafruit_BMP085 bmp;
  bool mpu_available;
  bool mag_available;
  bool bmp_available;
};

GY87Sensor::GY87Sensor() {
  mpu_available = false;
  mag_available = false;
  bmp_available = false;
}

bool GY87Sensor::enableMPUBypass() {
  // MPU6050의 I2C 마스터 모드 비활성화하고 패스스루 모드 활성화
  Wire.beginTransmission(0x68);  // MPU6050 I2C 주소
  Wire.write(0x6A);              // 레지스터 주소: USER_CTRL
  Wire.write(0x00);              // I2C 마스터 모드 비활성화
  byte error = Wire.endTransmission(true);
  
  if (error != 0) {
    Serial.println("MPU6050 접근 실패");
    return false;
  }
  
  Wire.beginTransmission(0x68);  // MPU6050 I2C 주소
  Wire.write(0x37);              // 레지스터 주소: INT_PIN_CFG
  Wire.write(0x02);              // I2C 패스스루 모드 활성화
  error = Wire.endTransmission(true);
  
  if (error != 0) {
    Serial.println("MPU6050 패스스루 설정 실패");
    return false;
  }
  
  Serial.println("MPU6050 I2C 패스스루 모드 활성화");
  return true;
}

bool GY87Sensor::init() {
  Wire.begin();
  
  // MPU6050 초기화
  if (mpu.begin()) {
    mpu_available = true;
    // 기본 설정
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
    Serial.println("MPU6050 초기화 성공");
    
    // HMC5883L 지자기 센서를 위한 MPU6050 I2C 패스스루 모드 활성화
    enableMPUBypass();
    delay(100); // 패스스루 모드 적용을 위한 지연
    
    // HMC5883L 초기화
    if (mag.begin()) {
      mag_available = true;
      Serial.println("HMC5883L 초기화 성공");
    } else {
      Serial.println("HMC5883L 초기화 실패 - 연결 확인 필요");
    }
    
    // BMP180 초기화
    if (bmp.begin()) {
      bmp_available = true;
      Serial.println("BMP180 초기화 성공");
    } else {
      Serial.println("BMP180 초기화 실패 - 연결 확인 필요");
    }
    
    return true;
  } else {
    Serial.println("GY87 초기화 실패 - MPU6050 연결 확인 필요");
    return false;
  }
}

bool GY87Sensor::read(GY87Data &data) {
  bool success = false;
  
  // MPU6050 데이터 읽기
  if (mpu_available) {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    
    // 데이터 복사
    data.acceleration.x = a.acceleration.x;
    data.acceleration.y = a.acceleration.y;
    data.acceleration.z = a.acceleration.z;
    
    data.gyroscope.x = g.gyro.x;
    data.gyroscope.y = g.gyro.y;
    data.gyroscope.z = g.gyro.z;
    
    data.temperature.temperature = temp.temperature;
    success = true;
  }
  
  // HMC5883L 데이터 읽기
  if (mag_available) {
    sensors_event_t event;
    mag.getEvent(&event);
    
    data.magnetic.x = event.magnetic.x;
    data.magnetic.y = event.magnetic.y;
    data.magnetic.z = event.magnetic.z;
    
    // 방위각 계산 (2D 방위각, 북쪽 기준)
    float heading = atan2(event.magnetic.y, event.magnetic.x);
    
    // 자기 편차 보정 (지역에 따라 다름)
    // 서울 기준 약 -8도 (2023년)
    float declinationAngle = -8.0 * PI / 180;
    heading += declinationAngle;
    
    // 음수 각도 보정
    if (heading < 0)
      heading += 2 * PI;
    
    // 360도 초과 보정
    if (heading > 2 * PI)
      heading -= 2 * PI;
    
    // 라디안에서 도(degree)로 변환
    data.magnetic.heading = heading * 180/PI;
  }
  
  // BMP180 데이터 읽기
  if (bmp_available) {
    data.pressure.pressure = bmp.readPressure();
    data.pressure.altitude = bmp.readAltitude();
  }
  
  return success;
}