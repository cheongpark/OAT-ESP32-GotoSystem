#include <Arduino.h>
#include <SoftwareSerial.h>

//SoftwareSerial Serial1(2, 3); // Arduino UNO용, Arduino Uno에선 Serial1 자체가 없기 때문에 선언 가능, ESP32에선 불가

void setup() {
  Serial.begin(115200);  // 디버그용
  Serial2.begin(2400);
  delay(1000);
  Serial.println("통신 테스트 시작...");
}

void loop() {
  // 버퍼 비우기
  while(Serial2.available()) {
    Serial2.read();
  }
  
  // 간단한 문자 전송
  Serial2.print("PING\n");
  Serial.println("PING 전송");
  
  // 응답 확인
  unsigned long startTime = millis();
  while (millis() - startTime < 2000) {
    if (Serial2.available()) {
      String response = Serial2.readStringUntil('\n');
      Serial.print("응답 바이트: ");
      for(int i=0; i < response.length(); i++) {
        Serial.print((int)response[i]);
        Serial.print(" ");
      }
      Serial.println();
      Serial.println("응답: " + response);
      break;
    }
  }
  
  delay(3000);
}