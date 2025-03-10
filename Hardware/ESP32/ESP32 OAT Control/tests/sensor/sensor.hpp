#include <Arduino.h>
#include <unity.h>

#include "./hall_sensor.hpp"
#include "./gps_sensor.hpp"
#include "./mpu_sensor.hpp"

enum {
  HALL,
  GPS,
  MPU
};

class SensorTest {
public:
  SensorTest();
  
  void init(int sensor_type);
  void test(int sensor_type);
  void test_hall_sensor();
  void test_gps_sensor();
  void test_mpu_sensor();

private:
  HallSensor hall;
  GPSSensor gps;
  MPUSensor mpu;
};

SensorTest::SensorTest() {
  
}

void SensorTest::init(int sensor_type) {
  switch (sensor_type) {
    case HALL:
      hall.init(5);
      break;
    case GPS:
      gps.init(Serial2);
      break;
    case MPU:
      mpu.init();
      break;
  }
}

void SensorTest::test(int sensor_type) {
  switch (sensor_type) {
    case HALL:
      test_hall_sensor();
      break;
    case GPS:
      test_gps_sensor();
      break;
    case MPU:
      test_mpu_sensor();
      break;
  }
}

void SensorTest::test_hall_sensor() { 
  for(int i = 0; i < 10; i++) {
    bool data;
    bool chk = hall.read(data);
    
    if(chk)
      Serial.println(data);

    TEST_ASSERT_TRUE(chk);
  }
}

void SensorTest::test_gps_sensor() {
  for(int i = 0; i < 10; i++) {
    uint8_t data;
    bool chk = gps.read(data);
    
    if(chk)
      Serial.println(data);

    TEST_ASSERT_TRUE(chk);
  }
}

void SensorTest::test_mpu_sensor() {
  for(int i = 0; i < 10; i++) {
    MPU6050 data;
    bool chk = mpu.read(data);
    
    if(chk) {
      // Teleplot
      Serial.print(">AccX:");
      Serial.println(data.acceleration.x);
      Serial.print(">AccY:");
      Serial.println(data.acceleration.y);
      Serial.print(">AccZ:");
      Serial.println(data.acceleration.z);
      
      Serial.print(">GyroX:");
      Serial.println(data.gyroscope.x);
      Serial.print(">GyroY:");
      Serial.println(data.gyroscope.y);
      Serial.print(">GyroZ:");
      Serial.println(data.gyroscope.z);
      
      // 여러 값을 한 줄에 출력하는 방법
      Serial.print(">Temp:");
      Serial.println(data.temperature.temperature);
    }
    TEST_ASSERT_TRUE(chk);
  }
}