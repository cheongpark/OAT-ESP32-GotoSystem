#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

class MPU6050 {
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
};

class MPUSensor {
public:
  MPUSensor();
  
  bool init();
  bool read(MPU6050 &data);

private:
  Adafruit_MPU6050 mpu;
};

MPUSensor::MPUSensor() {
  
}

bool MPUSensor::init() {
  Wire.begin();
  if (!mpu.begin()) {
    Serial.println("MPU6050 초기화 실패");
    return false;
  }
  
  // 기본 설정
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  
  Serial.println("MPU6050 초기화 성공");
  return true;
}

bool MPUSensor::read(MPU6050 &data) {
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
  
  return true;
}