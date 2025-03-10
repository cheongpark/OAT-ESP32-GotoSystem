#include <Arduino.h>
#include <unity.h>
#include <vector>

class OATConnectionTest {
public:
  OATConnectionTest();

  void init(HardwareSerial &oat_serial, std::vector<String> commands);
  void request();

private:
  HardwareSerial* oat_serial;
  std::vector<String> commands;

  unsigned long lastCommandTime = 0;
};

OATConnectionTest::OATConnectionTest() {
  // 기본 생성자
}

void OATConnectionTest::init(HardwareSerial &oat_serial, std::vector<String> commands) {
  this->oat_serial = &oat_serial;
  this->commands = commands;
}

void OATConnectionTest::request() {
    // Serial2 연결 초기화
    oat_serial->end();
    oat_serial->begin(2400);
    
    bool chk = true;
        
    oat_serial->print("#");
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