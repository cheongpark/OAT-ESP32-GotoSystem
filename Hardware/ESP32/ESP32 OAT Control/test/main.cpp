// Verbose Test로 실행해야 Serial 통신 출력 가능

#include <Arduino.h>
#include <unity.h>

#include "sensor/hall_sensor.cpp"
#include "sensor/gps_sensor.cpp"
#include "sensor/mpu_sensor.cpp"

HallSensor hall(5);
GPSSensor gps(23, 5); // RX TX
MPUSensor mpu;

void setUp() {
  // Unity를 위한 Setup 무조건 필요
}

void tearDown(void) {
  // Unity를 위한 TearDown 무조건 필요
}

/*
테스트 함수 만들 때

일반 Test로 빌드하고 실행되는 경우 Serial 통신의 출력은 나오지 않음.. Verbose Test만 가능

TEST_ASSERT 이런거 종류가 엄청 많으니 확인 https://github.com/ThrowTheSwitch/Unity/blob/master/docs/UnityAssertionsReference.md

함수 파라미터에 condition 넣는 부분이 있는데 이거는 true, false만 넣을 수 있음
true로 하면 출력될 때 PASSED가 뜨고 false를 넣으면 FAILED로 떠서 구분이 쉽게 되게 되어있음

void test_func() { // 반드시 파라미터는 아무것도 없어야 
  
}

RUN_TEST(test_func); // 이래야만 이걸 테스트로 인식하고 테스트를 시작함
*/

// Test Func
void test_hall_sensor() { 
  for(int i = 0; i < 10; i++) {
    bool data;
    bool chk = hall.read(data);
    
    if(chk)
      Serial.println(data);

    TEST_ASSERT_EQUAL(chk, true);
  }
}

void test_gps_sensor() {
  for(int i = 0; i < 10; i++) {
    uint8_t data;
    bool chk = gps.read(data);
    
    if(chk)
      Serial.println(data);

    TEST_ASSERT_EQUAL(chk, true);
  }
}

void test_mpu_sensor() {
  for(int i = 0; i < 10; i++) {
    MPU6050 data;
    bool chk = mpu.read(data);
    
    if(chk) {
      Serial.print("Acceleration : ");
      Serial.print(data.acceleration.x);
      Serial.print(", ");
      Serial.print(data.acceleration.y);
      Serial.print(", ");
      Serial.print(data.acceleration.z);
      Serial.println();

      Serial.print("Gyroscope : ");
      Serial.print(data.gyroscope.x);
      Serial.print(", ");
      Serial.print(data.gyroscope.y);
      Serial.print(", ");
      Serial.print(data.gyroscope.z);
      Serial.println();

      Serial.print("Temperature : ");
      Serial.print(data.temperature.temperature);
      Serial.println();
      Serial.println();
    }
    
    TEST_ASSERT_EQUAL(chk, true);
  }
}

void setup() {
  // Arduino를 위한 Setup 무조건 필요

  Serial.begin(115200);
  UNITY_BEGIN();
}

void loop() {
  // Arduino를 위한 Loop 무조건 필요

  // RUN_TEST(test_hall_sensor);
  // RUN_TEST(test_gps_sensor);
  RUN_TEST(test_mpu_sensor);

  UNITY_END();
}