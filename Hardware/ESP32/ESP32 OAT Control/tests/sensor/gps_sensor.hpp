#pragma once

#include <Arduino.h>
#include <unity.h>
#include <TinyGPSPlus.h>

class GPSSensor {
public:
  GPSSensor() {}
  
  void init(HardwareSerial &serial, uint32_t baud = 9600, int rxPin = -1, int txPin = -1) {
    this->serial = &serial;
    
    // ESP32에서 Serial 핀 설정
    Serial.println("GPS 센서 초기화 - RX:" + String(rxPin) + ", TX:" + String(txPin) + ", 속도:" + String(baud));
    serial.begin(baud, SERIAL_8N1, rxPin, txPin);
    
    // 헤더 출력
    printHeader();
  }
  
  void printHeader() {
    Serial.println(F("Sats HDOP  Latitude   Longitude   Fix  Date       Time     Alt    Course Speed Card  Chars Sentences Checksum"));
    Serial.println(F("           (deg)      (deg)       Age                      (m)    (deg)  (km/h)      RX    RX        Fail"));
    Serial.println(F("------------------------------------------------------------------------------------------------------"));
  }
  
  bool update() {
    bool newData = false;
    while (serial->available() > 0) {
      char c = serial->read();
      if (gps.encode(c)) {
        newData = true;
      }
    }
    return newData;
  }
  
  void printData() {
    printInt(gps.satellites.value(), gps.satellites.isValid(), 5);
    printFloat(gps.hdop.hdop(), gps.hdop.isValid(), 6, 1);
    printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
    printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);
    printInt(gps.location.age(), gps.location.isValid(), 5);
    printDateTime(gps.date, gps.time);
    printFloat(gps.altitude.meters(), gps.altitude.isValid(), 7, 2);
    printFloat(gps.course.deg(), gps.course.isValid(), 7, 2);
    printFloat(gps.speed.kmph(), gps.speed.isValid(), 6, 2);
    printStr(gps.course.isValid() ? TinyGPSPlus::cardinal(gps.course.deg()) : "*** ", 6);
    printInt(gps.charsProcessed(), true, 6);
    printInt(gps.sentencesWithFix(), true, 10);
    printInt(gps.failedChecksum(), true, 9);
    Serial.println();
  }
  
  void smartDelay(unsigned long ms) {
    unsigned long start = millis();
    do {
      update();
    } while (millis() - start < ms);
  }
  
  TinyGPSPlus* getGPS() {
    return &gps;
  }

private:
  HardwareSerial* serial;
  TinyGPSPlus gps;
  
  static void printFloat(float val, bool valid, int len, int prec) {
    if (!valid) {
      while (len-- > 1)
        Serial.print('*');
      Serial.print(' ');
    }
    else {
      Serial.print(val, prec);
      int vi = abs((int)val);
      int flen = prec + (val < 0.0 ? 2 : 1); // . and -
      flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
      for (int i=flen; i<len; ++i)
        Serial.print(' ');
    }
  }
  
  static void printInt(unsigned long val, bool valid, int len) {
    char sz[32] = "*****************";
    if (valid)
      sprintf(sz, "%ld", val);
    sz[len] = 0;
    for (int i=strlen(sz); i<len; ++i)
      sz[i] = ' ';
    if (len > 0) 
      sz[len-1] = ' ';
    Serial.print(sz);
  }
  
  static void printDateTime(TinyGPSDate &d, TinyGPSTime &t) {
    if (!d.isValid()) {
      Serial.print(F("********** "));
    }
    else {
      char sz[32];
      sprintf(sz, "%02d/%02d/%02d ", d.month(), d.day(), d.year());
      Serial.print(sz);
    }
    
    if (!t.isValid()) {
      Serial.print(F("******** "));
    }
    else {
      char sz[32];
      sprintf(sz, "%02d:%02d:%02d ", t.hour(), t.minute(), t.second());
      Serial.print(sz);
    }
  }
  
  static void printStr(const char *str, int len) {
    int slen = strlen(str);
    for (int i=0; i<len; ++i)
      Serial.print(i<slen ? str[i] : ' ');
  }
};