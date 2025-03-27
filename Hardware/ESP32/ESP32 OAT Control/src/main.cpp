#include "main.hpp"

void setup() {
    // 시리얼 통신 초기화
    Serial.begin(115200);
    LOG(LOG_INFO, "ESP32 OAT Control 시스템 시작");
    
    // WiFi 설정 & 웹서버 설정
    setupWiFi();
    web_manager.init();
    
    // OAT 연결
    {
        mount_serial.init(OAT_SERIAL, OAT_BAUD_RATE, OAT_SERIAL_RX, OAT_SERIAL_TX);
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

    // Sensor 연결 및 설정
    {
        gps_sensor.init(GPS_SERIAL, GPS_BAUD_RATE, GPS_SERIAL_RX, GPS_SERIAL_TX);
        bool is_connect = gps_sensor.connect();
        if(is_connect) {
            LOG(LOG_INFO, "GPS 센서가 연결되었습니다.");
        }
        else {
            LOG(LOG_ERROR, "GPS 센서 연결에 실패했습니다.");
        }
    }
    {
        bool is_connect = imu_sensor.init(IMU_I2C_SDA, IMU_I2C_SCL);
        if(is_connect) {
            LOG(LOG_INFO, "IMU 센서가 연결되었습니다.");
        }
        else {
            LOG(LOG_ERROR, "IMU 센서 연결에 실패했습니다.");
        }
    }

    // TODO 웹에서 변경 가능하도록 만들기, 현재는 임시로 설정한 것
    gps_sensor.setCurrentLocation(37.5512, 126.9882, 262.0);
    gps_sensor.setCurrentUTCTime(2024, 1, 15, 12, 30, 0);
}

void loop() {
    // DNS 서버 처리
    dns_server.processNextRequest();
    
    // TODO 테스트용 나중에 지우셈
    if(false) {
        if (gps_sensor.hasValidLocation()) {
            String location = gps_sensor.getFormattedLocation();
            Serial.println("현재 위치: " + location);
        }
        else {
            String location = gps_sensor.getFormattedLocation();
            Serial.println("위치 측정 불가능, 기본 위치: " + location);
        }
        
        if (gps_sensor.hasValidTime()) {
            String utc_time = gps_sensor.getFormattedUTCDateTime();
            String local_time = gps_sensor.getFormattedLocalDateTime();
            Serial.println("UTC 시간: " + utc_time);
            Serial.println("로컬 시간: " + local_time);
        }
        else {
            String utc_time = gps_sensor.getFormattedUTCDateTime();
            String local_time = gps_sensor.getFormattedLocalDateTime();
            Serial.println("시간 측정 불가능, 기본 시간: " + utc_time);
        }
    }
    else {
        if (imu_sensor.isConnected()) {
            Serial.println("가속도: " + imu_sensor.getAccelerationString());
            Serial.println("자이로스코프: " + imu_sensor.getGyroscopeString());
            Serial.println("온도: " + imu_sensor.getTemperatureString());
            Serial.println("고도: " + imu_sensor.getAltitudeString());
        } else {
            Serial.println("IMU 센서가 연결되지 않았습니다.");
        }
    }

    delay(1000);
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

    // mDNS 시작
    if (MDNS.begin(WIFI_MDNS_HOSTNAME)) {
        LOG(LOG_INFO, "mDNS 시작됨 - " + String(WIFI_MDNS_HOSTNAME) + ".local");
        MDNS.addService("http", "tcp", 80);
    } else {
        LOG(LOG_ERROR, "mDNS 시작 실패");
    }

    // DNS 서버 시작 (모든 요청을 ESP32로 리다이렉트)
    if (dns_server.start(53, "*", WiFi.softAPIP())) {
        LOG(LOG_INFO, "DNS 서버 시작됨");
    } else {
        LOG(LOG_ERROR, "DNS 서버 시작 실패");
    }
}