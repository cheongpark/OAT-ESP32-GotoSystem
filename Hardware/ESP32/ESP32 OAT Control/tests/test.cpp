// Verbose Test로 실행해야 Serial 통신 출력 가능

#include <Arduino.h>
#include <unity.h>

// OAT Connection Test : 1
// Sensor Test : 2
#define TEST_NUM 1 

#if TEST_NUM == 1
#include "oat_connection_test/oat_connection_test.hpp"
#elif TEST_NUM == 2
#include "sensor/sensor.hpp"
#endif

#if TEST_NUM == 1
OATConnectionTest oat_test;

void test_oat() {
  oat_test.request();
}
#elif TEST_NUM == 2
SensorTest sensor_test;

void test_sensor() {
  sensor_test.test(MPU);
}
#endif

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


/*
나중에 ISR 핸들에 대해서 찾아보셈
ESP32는 그걸 지원하는 듯 모양인데..
대충 C++ 할 때 GUI에서 어떤걸 눌렀을 때
다른 작업을 하고 있더라도 바로 그 버튼의 처리로 넘어가는
핸들 기능을 할 수 있나봄
프로그래밍이 조금 어려워지긴 하는데.. 핸들 기능을 사용하면 모듈로 관리나
더 빠른 반영이 가능해서 이 방법으로 해야할 듯
*/

// Test Func


void setup() {
  // Arduino를 위한 Setup 무조건 필요

  Serial.begin(115200);

  #if TEST_NUM == 1
  oat_test.init(Serial2, {"GVP", "GVN", "XGM"});
  #elif TEST_NUM == 2
  sensor_test.init(MPU);
  #endif

  UNITY_BEGIN();
}

void loop() {
  // Arduino를 위한 Loop 무조건 필요

  #if TEST_NUM == 1
  RUN_TEST(test_oat);
  #elif TEST_NUM == 2
  RUN_TEST(test_sensor);
  #endif

  UNITY_END();
}