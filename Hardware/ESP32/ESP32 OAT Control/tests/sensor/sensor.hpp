#pragma once

#include <Arduino.h>
#include <unity.h>

#include "./hall_sensor.hpp"
#include "./gps_sensor.hpp"
#include "./mpu_sensor.hpp"
#include "./gy87_sensor.hpp"

enum SensorType {
  SENSOR_HALL = 0,
  SENSOR_GPS = 1,
  SENSOR_MPU = 2,
  SENSOR_GY87 = 3
};

class SensorTest {
public:
  SensorTest();
  
  void init(int sensor_type);
  void test(int sensor_type);
  void test_hall_sensor();
  void test_gps_sensor();
  void test_mpu_sensor();
  void test_gy87_sensor();

private:
  HallSensor hall;
  GPSSensor gps;
  MPUSensor mpu;
  GY87Sensor gy87;
};

SensorTest::SensorTest() {
  
}

void SensorTest::init(int sensor_type) {
  switch (sensor_type) {
    case SENSOR_HALL:
      hall.init(5);
      break;
    case SENSOR_GPS:
      gps.init(Serial2);
      break;
    case SENSOR_MPU:
      mpu.init();
      break;
    case SENSOR_GY87:
      gy87.init();
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
    case SENSOR_MPU:
      test_mpu_sensor();
      break;
    case SENSOR_GY87:
      test_gy87_sensor();
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
  // 모든 라이브러리 사용 없이 단순히 Serial2 데이터 모니터링
  Serial.println("===== Serial2 Raw Data Monitor =====");
  
  // Serial2 초기화
  Serial2.begin(9600);
  
  // 변수 초기화
  String line = "";
  unsigned long lastDisplayTime = 0;
  const unsigned long displayInterval = 3000; // 3초마다 상태 표시
  
  // 무한 루프로 데이터 출력
  while(true) {
    // 현재 시간 체크
    unsigned long currentTime = millis();
    
    // 일정 간격으로 모니터링 상태 표시
    if(currentTime - lastDisplayTime > displayInterval) {
      Serial.println("\n----- 모니터링 중... -----");
      lastDisplayTime = currentTime;
    }
    
    // 데이터 읽기
    if(Serial2.available()) {
      char c = Serial2.read();
      
      // 줄바꿈 문자가 오면 라인 출력 후 초기화
      if(c == '\n' || c == '\r') {
        if(line.length() > 0) {
          Serial.println(line);
          line = "";
        }
      } else {
        // 줄바꿈이 아니면 문자 추가
        line += c;
        
        // 라인이 너무 길어지면 강제 출력
        if(line.length() > 80) {
          Serial.println(line);
          line = "";
        }
      }
      
      // 원시 데이터도 출력 (옵션)
      Serial.write(c);
    }
    
    // 약간의 지연을 주어 CPU 부하 감소
    delay(1);
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

void SensorTest::test_gy87_sensor() {
  GY87Data data;
  bool chk;
  
  // 초기 데이터 수집
  for(int i = 0; i < 3; i++) {
    chk = gy87.read(data);
    if(chk) {
      data.printTelePlot();
    }
    TEST_ASSERT_TRUE(chk);
    delay(100);
  }
  
  // 무한 루프로 텔레플롯 데이터 계속 송신
  Serial.println("계속해서 데이터 송신 중...");
  while(true) {
    chk = gy87.read(data);
    if(chk) {
      data.printTelePlot();
    }
    delay(100); // 100ms 간격으로 데이터 수집 (10Hz)
  }
}