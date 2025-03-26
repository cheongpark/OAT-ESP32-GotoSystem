#pragma once

#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>

#include "secrets.hpp"
#include "page.hpp"
#include "log.hpp"
#include "settings.hpp"
#include "mount/serial.hpp"
#include "mount/data.hpp"
#include "management/initialize.hpp"
#include "management/web_manager.hpp"

AsyncWebServer server(80);  // 80 포트에서 웹 서버 실행
Mount::Serial& mount_serial = Mount::Serial::getInstance();
Mount::Data& mount_data = Mount::Data::getInstance();

Initialize initialize;
WEB::WebManager& web_manager = WEB::WebManager::getInstance();

void setupWiFi();