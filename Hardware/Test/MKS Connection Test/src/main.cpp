#include <Arduino.h>

void setup() {
  Serial.begin(115200);  // 디버그용
  // ESP32 핀 명시적으로 지정 (RX, TX)
  Serial1.begin(2400);  // RX_PIN, TX_PIN은 실제 사용 핀으로 변경
  delay(1000);
  Serial.println("수신 장치 준비됨");
}

void loop() {
  // 수신 데이터 확인
  if (Serial1.available()) {
    String received = Serial1.readStringUntil('\n');
    Serial.println("수신: " + received);
    
    // 버퍼 비우기
    delay(10);
    
    // 응답 보내기
    Serial1.print("PONG\n");
    Serial.println("PONG 응답 전송");
  }
}