#pragma once

#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <DNSServer.h>
#include <ESPmDNS.h>

#include "secrets.hpp"
#include "page.hpp"
#include "log.hpp"
#include "settings.hpp"
#include "mount/serial.hpp"
#include "mount/data.hpp"
#include "management/initialize.hpp"
#include "management/web_manager.hpp"
#include "sensor/gps.hpp"
#include "sensor/imu.hpp"

// Web Server
AsyncWebServer server(80);  // 80 포트에서 웹 서버 실행
Initialize initialize;
DNSServer dns_server;
WEB::WebManager& web_manager = WEB::WebManager::getInstance();

// Mount
Mount::Serial& mount_serial = Mount::Serial::getInstance();
Mount::Data& mount_data = Mount::Data::getInstance();

// Sensor
SENSOR::GPS& gps_sensor = SENSOR::GPS::getInstance();
SENSOR::IMU& imu_sensor = SENSOR::IMU::getInstance();

void setupWiFi();