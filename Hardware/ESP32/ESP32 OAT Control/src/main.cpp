#include "main.hpp"

void setup() {
    // 시리얼 통신 초기화
    Serial.begin(115200);
    LOG(LOG_INFO, "ESP32 OAT Control 시스템 시작");
    
    // WiFi 설정 & 웹서버 설정
    setupWiFi();
    setupWebServer();

    // OAT 연결
    mount_serial.init(OAT_SERIAL, OAT_BAUD_RATE);
    bool is_connect = mount_serial.connect();
    if(is_connect) 
        LOG(LOG_INFO, "OAT가 연결되었습니다.");
    else
        LOG(LOG_ERROR, "OAT 연결에 실패했습니다.");

    // 초기화
    initialize.init();
}

void loop() {

}

// WiFi 연결 설정
void setupWiFi() {
    LOG(LOG_INFO, "Running AP MODE");
    WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);

    LOG(LOG_INFO, "IP Address: " + WiFi.softAPIP().toString());
}

String processor(const String& var) {
    if(var == "GPSINFO")
        return F("asdsdasadasdasd");
    if(var == "FIRMWAREMODEL")
        return F(mount_data.get_info_product_name().c_str());
    if(var == "FIRMWAREVERSION")
        return F(mount_data.get_info_firmware_version().c_str());
    return String();
}

// 웹 서버 설정
void setupWebServer() {
    // 루트 경로 처리
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        String html = String(index_html);
        html.replace("%STYLE_PLACEHOLDER%", style_css);
        html.replace("%SCRIPT_PLACEHOLDER%", script_js);
        request->send(200, "text/html", html, processor);
    });
    
    server.on("/sensordata", HTTP_GET, [](AsyncWebServerRequest *request) {

    });
    
    server.on("/systeminfo", HTTP_GET, [](AsyncWebServerRequest *request) {

    });
    
    server.on("/led/toggle", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "application/json");
    });
    
    // LED 상태 설정 엔드포인트
    server.on("/led", HTTP_GET, [](AsyncWebServerRequest *request) {

    });
    
    // 서버 시작
    server.begin();
    LOG(LOG_INFO, "웹 서버 시작됨");
}