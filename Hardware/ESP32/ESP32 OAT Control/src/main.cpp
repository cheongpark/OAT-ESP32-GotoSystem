#include <Arduino.h>

bool initialized = false;
unsigned long lastCommandTime = 0;

void setup() {
    Serial.begin(115200);
    
    Serial.println("Start");
}

void loop() {
    String commands[] = {"GVP", "GVN", "XGM"};

    // Serial2 연결 초기화
    Serial2.end();  // 기존 연결 종료
    Serial2.begin(2400, SERIAL_8N1, 16, 17);  // 매번 새로 초기화
    delay(500);  // 초기화 후 안정화 시간 증가
    
    Serial.println("OAT에 연결을 시도합니다.");
    
    // 연결 확인 방식 변경 - 항상 명령 전송 시도
    Serial.println("OAT에 연결이 완료되었습니다.");
    
    Serial2.print("#");
    for (const String& cmd : commands) {
        Serial2.print(":" + cmd + "#");  // Serial2로 명령 전송
        Serial.println(cmd + " 명령어를 보냈습니다.");
        lastCommandTime = millis();
        
        // 응답 대기
        unsigned long startTime = millis();
        bool responseReceived = false;
        
        while (millis() - startTime < 3000) {  // 3초 타임아웃
            if (Serial2.available() > 0) {
                responseReceived = true;
                break;
            }
            delay(10);
        }
        
        if (responseReceived) {
            String response = Serial2.readStringUntil('\n');
            
            Serial.print("응답 바이트: ");
            for(int i=0; i < response.length(); i++) {
                Serial.print((int)response[i]);
                Serial.print(" ");
            }
            Serial.println();
            
            Serial.println("응답: " + response);
        } else {
            Serial.println("응답 없음: 타임아웃");
        }
        
        delay(1000);  // 명령 사이 대기
    }
    
    Serial.println("다시 시도합니다. 10초 대기");
    delay(10000);
    
    Serial.println();
}
