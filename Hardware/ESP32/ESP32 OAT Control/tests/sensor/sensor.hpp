#pragma once

#include <Arduino.h>
#include <unity.h>

#include "./hall_sensor.hpp"
#include "./gps_sensor.hpp"
#include "./imu_sensor.hpp"

enum SensorType {
  SENSOR_HALL = 0,
  SENSOR_GPS = 1,
  SENSOR_IMU = 2
};

class SensorTest {
public:
  SensorTest();
  
  void init(int sensor_type);
  void test(int sensor_type);
  void test_hall_sensor();
  void test_gps_sensor();
  void test_imu_sensor();

private:
  HallSensor hall;
  GPSSensor gps;
  IMUSensor imu;
};

SensorTest::SensorTest() {
  
}

void SensorTest::init(int sensor_type) {
  switch (sensor_type) {
    case SENSOR_HALL:
      hall.init(5);
      break;
    case SENSOR_GPS:
      gps.init(Serial2, 9600, 19, 23);
      break;
    case SENSOR_IMU:
      imu.init();
      break;
  }
}

void SensorTest::test(int sensor_type) {
  switch (sensor_type) {
    case SENSOR_HALL:
      test_hall_sensor();
      break;
    case SENSOR_GPS:
      test_gps_sensor();
      break;
    case SENSOR_IMU:
      test_imu_sensor();
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
  Serial.println("===== GPS 테스트 시작 =====");
  Serial.println("GPS 데이터 수신 대기 중...");
  
  // 변수 초기화
  unsigned long lastDataTime = 0;
  unsigned long lastWarningTime = 0;
  const unsigned long WARNING_INTERVAL = 10000;  // 10초마다 경고
  
  // 무한 루프로 데이터 출력
  while(true) {
    // GPS 데이터 업데이트 시도
    bool updated = gps.update();
    unsigned long currentTime = millis();
    
    // 데이터가 업데이트되면 출력
    if (updated) {
      gps.printData();
      lastDataTime = currentTime;
    }
    
    // 10초 동안 데이터가 없으면 경고 메시지
    if (currentTime - lastDataTime > WARNING_INTERVAL && 
        currentTime - lastWarningTime > WARNING_INTERVAL) {
      Serial.println("GPS 데이터를 수신할 수 없습니다. 연결 및 안테나를 확인하세요.");
      lastWarningTime = currentTime;
    }
    
    // 스마트 딜레이 (데이터를 계속 수신하면서 대기)
    gps.smartDelay(1000);
  }
}

void SensorTest::test_imu_sensor() {
  IMUData data;
  bool chk;
  
  // 초기 데이터 수집
  for(int i = 0; i < 3; i++) {
    chk = imu.read(data);
    if(chk) {
      data.printTelePlot();
    }
    TEST_ASSERT_TRUE(chk);
    delay(100);
  }
  
  // 무한 루프로 텔레플롯 데이터 계속 송신
  Serial.println("계속해서 데이터 송신 중...");
  while(true) {
    chk = imu.read(data);
    if(chk) {
      data.printTelePlot();
    }
    delay(100); // 100ms 간격으로 데이터 수집 (10Hz)
  }
}