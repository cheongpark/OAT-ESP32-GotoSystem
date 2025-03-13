#pragma once

#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>

#include "secrets.hpp"
#include "page.hpp"
#include "log.hpp"
#include "mount_serial.hpp"
#include "settings.hpp"

extern AsyncWebServer server;

void setupWiFi();
void setupWebServer();
String processor(const String& var);