#include "mount/serial.hpp"

namespace Mount {
    Serial& Serial::getInstance() {
        static Serial instance;
        return instance;
    }

    void Serial::init(HardwareSerial &mnt_serial, const unsigned long baud_rate, const int rxPin, const int txPin) {
        this->mnt_serial = &mnt_serial;
        this->baud_rate = baud_rate;

        this->mnt_serial->begin(baud_rate, SERIAL_8N1, rxPin, txPin);
    }

    bool Serial::connect() {
        this->mnt_serial->end();
        this->mnt_serial->begin(this->baud_rate);

        delay(1000); // connect delay

        // 시리얼 버퍼 비우기
        while(this->mnt_serial->available()) {
            this->mnt_serial->read();
        }

        initialized = true;

        return true;
    }

    Response Serial::request(const String command, bool is_response) {
        if (!initialized) {
            LOG(LOG_ERROR, "시리얼이 초기화되지 않았습니다.");
            return {false, ""};
        }

        this->mnt_serial->print(":" + command + "#");  // Serial2로 명령 전송
        LOG(LOG_INFO, "\":" + command + "#\"" + " 명령어를 보냈습니다.");
        
        if (!is_response) {
            return {true, ""};
        }

        // 응답 대기
        unsigned long start_time = millis();
        bool response_received = false;
        String response;

        while (millis() - start_time < 2000) {  // 2초 타임아웃
            if (this->mnt_serial->available() > 0) {
                response_received = true;
                break;
            }
            delay(10);
        }
        
        if (response_received) {
            response = this->mnt_serial->readStringUntil('\n');

            // 마지막 # 문자가 있으면 제거
            if (response.endsWith("#")) 
                response.remove(response.length() - 1);

            LOG(LOG_INFO, "응답: " + response);
            return {true, response};
        } else {
            LOG(LOG_ERROR, "응답 없음: 타임아웃");
            return {false, "timeout"};
        }
    }
}
