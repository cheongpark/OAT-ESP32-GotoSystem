#include "main.hpp"

void setup() {
    // 시리얼 통신 초기화
    Serial.begin(115200);
    LOG(LOG_INFO, "ESP32 OAT Control 시스템 시작");
    
    // WiFi 설정 & 웹서버 설정
    setupWiFi();
    web_manager.init();

    // OAT 연결
    mount_serial.init(OAT_SERIAL, OAT_BAUD_RATE);
    bool is_connect = mount_serial.connect();
    if(is_connect) {
        LOG(LOG_INFO, "OAT가 연결되었습니다.");

        // 초기화
        initialize.init();
    }
    else {
        LOG(LOG_ERROR, "OAT 연결에 실패했습니다.");
    }
}

void loop() {

}

// WiFi 연결 설정
void setupWiFi() {
    LOG(LOG_INFO, "Running AP MODE");
    
    // IP 주소 설정
    IPAddress local_IP;
    IPAddress gateway;
    IPAddress subnet;
    
    local_IP.fromString(WIFI_IP);
    gateway.fromString(WIFI_GATEWAY);
    subnet.fromString(WIFI_SUBNET);
    
    // IP 설정 적용
    WiFi.softAPConfig(local_IP, gateway, subnet);
    
    // WiFi AP 시작
    WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);

    LOG(LOG_INFO, "IP Address: " + WiFi.softAPIP().toString());
}