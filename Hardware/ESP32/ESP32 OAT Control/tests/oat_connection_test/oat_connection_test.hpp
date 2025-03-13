#include <Arduino.h>
#include <unity.h>
#include <vector>

class OATConnectionTest {
public:
  OATConnectionTest();

  bool init(HardwareSerial &oat_serial, std::vector<String> commands);
  void request();
  bool isConnected() { return oat_serial != nullptr && oat_serial->available() >= 0; }

private:
  HardwareSerial* oat_serial;
  std::vector<String> commands;

  unsigned long lastCommandTime = 0;
  bool initialized = false;
};

OATConnectionTest::OATConnectionTest() {
  // 기본 생성자
}

bool OATConnectionTest::init(HardwareSerial &oat_serial, std::vector<String> commands) {
  this->oat_serial = &oat_serial;
  this->commands = commands;

  // Serial 연결 초기화
  this->oat_serial->end();
  this->oat_serial->begin(2400);
  delay(1000);  // 시리얼 포트 초기화를 위한 대기 시간

  // 시리얼 버퍼 비우기
  while(this->oat_serial->available()) {
    this->oat_serial->read();
  }

  initialized = true;
  return isConnected();
}

void OATConnectionTest::request() {
  if (!initialized || !isConnected()) {
    Serial.println("시리얼이 초기화되지 않았거나 연결되지 않았습니다.");
    TEST_ASSERT_TRUE(false);
    return;
  }

  bool chk = true;
        
  for (const String& cmd : commands) {
    oat_serial->print(":" + cmd + "#");  // Serial2로 명령 전송
    Serial.println(cmd + " 명령어를 보냈습니다.");
    lastCommandTime = millis();
    
    // 응답 대기
    unsigned long startTime = millis();
    bool responseReceived = false;
    
    while (millis() - startTime < 2000) {  // 2초 타임아웃
      if (oat_serial->available() > 0) {
        responseReceived = true;
        break;
      }
      delay(10);
    }
    
    if (responseReceived) {
      String response = oat_serial->readStringUntil('\n');
      
      Serial.print("응답 바이트: ");
      for(int i=0; i < response.length(); i++) {
        Serial.print((int)response[i]);
        Serial.print(" ");
      }
      Serial.println();
      
      Serial.println("응답: " + response);
    } else {
      Serial.println("응답 없음: 타임아웃");
      chk = false;
    }
    
    delay(1000);  // 명령 사이 대기
  }

  TEST_ASSERT_TRUE(chk);
}