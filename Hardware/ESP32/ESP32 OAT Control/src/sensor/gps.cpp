#include "sensor/gps.hpp"

namespace SENSOR {
    // ===== 생성자 구현 =====
    GPS::GPS() : 
#if !GPS_FAKE_DATA
        _gps_serial(nullptr), 
#endif
        _baud_rate(0),
        _initialized(false),
        _connected(false),
        _latitude(GPS_DEFAULT_LATITUDE),
        _longitude(GPS_DEFAULT_LONGITUDE),
        _altitude(GPS_DEFAULT_ALTITUDE),
        _satellites(0),
        _hdop(0), 
        _utc_year(2023), _utc_month(1), _utc_day(1),
        _utc_hour(0), _utc_minute(0), _utc_second(0),
        _local_year(2023), _local_month(1), _local_day(1),
        _local_hour(0), _local_minute(0), _local_second(0),
        _default_latitude(GPS_DEFAULT_LATITUDE),
        _default_longitude(GPS_DEFAULT_LONGITUDE),
        _default_altitude(GPS_DEFAULT_ALTITUDE),
        _default_year(2023), _default_month(1), _default_day(1),
        _default_hour(0), _default_minute(0), _default_second(0),
        _last_update_time(0),
        _last_gps_process_time(0),
        _gps_task_handle(nullptr)
    {
    }

    // ===== 싱글톤 인스턴스 가져오기 =====
    GPS& GPS::getInstance() {
        static GPS instance;
        return instance;
    }
    
    // ===== GPS 작업 처리 태스크 함수 =====
    void GPS::gpsTask(void* parameter) {
        GPS* gps_instance = static_cast<GPS*>(parameter);
        
        for (;;) {
            // GPS 데이터 처리
            if (gps_instance->isConnected()) {
                // 실제 GPS 데이터 처리 또는 가짜 데이터 생성
#if GPS_FAKE_DATA
                gps_instance->generateFakeData();
#else
                gps_instance->processGPSData();
#endif
                
                // 데이터 유효성 체크
                gps_instance->validateData();
            }
            
            // 업데이트 간격 대기
            vTaskDelay(pdMS_TO_TICKS(GPS_UPDATE_INTERVAL_MS));
        }
    }

    // ===== 초기화 함수 =====
    void GPS::init(HardwareSerial &gps_serial, const unsigned long baud_rate, const int rxPin, const int txPin) {
#if !GPS_FAKE_DATA
        this->_gps_serial = &gps_serial;
#endif
        this->_baud_rate = baud_rate;

        // 시리얼 포트 초기화 (가짜 데이터 모드가 아닐 때만 실제로 시리얼 포트 초기화)
#if !GPS_FAKE_DATA
        this->_gps_serial->begin(baud_rate, SERIAL_8N1, rxPin, txPin);
#endif

#if GPS_FAKE_DATA
        // AP 모드에서는 인터넷 연결이 없으므로 NTP 서버에 접근할 수 없음
        // 바로 기본 시간으로 ESP32 시스템 시간 설정
        LOG(LOG_INFO, "가짜 데이터 모드: 시스템 시간을 기본값으로 설정합니다");
        setDefaultSystemTime();
#endif
        
        // 초기 현재 시간 설정
        _last_update_time = millis();
        _last_gps_process_time = millis();
        
        // GPS 태스크 시작
        xTaskCreate(
            gpsTask,             // 태스크 함수
            "GPS_Task",          // 태스크 이름
            4096,                // 스택 크기
            this,                // 태스크 매개변수
            1,                   // 우선순위
            &_gps_task_handle    // 태스크 핸들
        );
        
#if GPS_FAKE_DATA
        LOG(LOG_INFO, "GPS 초기화 완료: 모드=가짜데이터(ESP32 내부 시계 사용), 속도=" + String(baud_rate) + 
            ", RX핀=" + String(rxPin) + ", TX핀=" + String(txPin));
#else
        LOG(LOG_INFO, "GPS 초기화 완료: 모드=실제GPS(신호 수신 대기 중), 속도=" + String(baud_rate) + 
            ", RX핀=" + String(rxPin) + ", TX핀=" + String(txPin));
#endif
        _initialized = true;
    }
    
    // ===== 연결 관련 함수 =====
    bool GPS::connect() {
        if (!_initialized) {
            LOG(LOG_ERROR, "GPS가 초기화되지 않았습니다.");
            return false;
        }
        
        // 가짜 데이터 모드에서는 실제 연결 시도 안함
#if !GPS_FAKE_DATA
        this->_gps_serial->end();
        this->_gps_serial->begin(this->_baud_rate);

        delay(1000); // 연결 지연

        // 시리얼 버퍼 비우기
        while(this->_gps_serial->available()) {
            this->_gps_serial->read();
        }
#endif
        
        _connected = true;
#if GPS_FAKE_DATA
        LOG(LOG_INFO, "GPS 연결 완료 (가짜데이터 모드 - 자동 시간 생성)");
#else
        LOG(LOG_INFO, "GPS 연결 완료 (실제GPS 모드 - 신호 수신 대기 중)");
#endif

        return this->isConnected();
    }

    bool GPS::isConnected() {
#if GPS_FAKE_DATA
        // 가짜 데이터 모드에서는 항상 연결된 것으로 간주
        return _initialized && _connected;
#else
        return _initialized && _connected && this->_gps_serial != nullptr;
#endif
    }
    
    void GPS::disconnect() {
        if (_gps_task_handle != nullptr) {
            vTaskDelete(_gps_task_handle);
            _gps_task_handle = nullptr;
        }
        
#if !GPS_FAKE_DATA
        // 실제 GPS 모드일 경우에만 시리얼 종료
        if (this->_gps_serial != nullptr) {
            this->_gps_serial->end();
        }
#endif
        
        _connected = false;
        LOG(LOG_INFO, "GPS 연결 해제");
    }
    
    // ===== GPS 데이터 처리 함수 =====
#if !GPS_FAKE_DATA
    void GPS::processGPSData() {
        unsigned long current_time = millis();
        bool updated = false;
        
        // GPS 데이터 처리
        while (_gps_serial->available() > 0) {
            char c = _gps_serial->read();
            if (_gps.encode(c)) {
                // 위치 정보 업데이트
                if (_gps.location.isValid()) {
                    _latitude = _gps.location.lat();
                    _longitude = _gps.location.lng();
                    _last_update_time = current_time;
                    updated = true;
                }
                
                // 고도 정보 업데이트
                if (_gps.altitude.isValid()) {
                    _altitude = _gps.altitude.meters();
                    _last_update_time = current_time;
                    updated = true;
                }
                
                // 위성 및 HDOP 정보 업데이트
                if (_gps.satellites.isValid()) {
                    _satellites = _gps.satellites.value();
                }
                
                if (_gps.hdop.isValid()) {
                    _hdop = _gps.hdop.hdop();
                }
                
                // 날짜/시간 정보 업데이트 (UTC 시간)
                if (_gps.date.isValid() && _gps.time.isValid()) {
                    // UTC 시간 저장
                    _utc_year = _gps.date.year();
                    _utc_month = _gps.date.month();
                    _utc_day = _gps.date.day();
                    _utc_hour = _gps.time.hour();
                    _utc_minute = _gps.time.minute();
                    _utc_second = _gps.time.second();
                    
                    // UTC 시간을 로컬 시간으로 변환
                    convertUTCtoLocal();
                    
                    _last_update_time = current_time;
                    _last_gps_process_time = current_time; // 실제 시간 데이터를 받았을 때만 last_gps_process_time 갱신
                    updated = true;
                    
                    LOG(LOG_INFO, "GPS 시간 수신: " + String(_utc_year) + "-" + 
                                   String(_utc_month) + "-" + String(_utc_day) + " " + 
                                   String(_utc_hour) + ":" + String(_utc_minute) + ":" + String(_utc_second) + " UTC");
                }
            }
        }
        
        // 데이터가 갱신되었으면 로그 출력
        if (updated) {
            LOG(LOG_INFO, "GPS 데이터 갱신: 위치=" + String(_latitude, 6) + ", " + String(_longitude, 6) + 
                          ", 고도=" + String(_altitude, 2) + "m, 위성=" + String(_satellites) + 
                          ", HDOP=" + String(_hdop));
        }
        
        // _last_gps_process_time 갱신하지 않음 - 이렇게 해야 실제 데이터가 없을 때도 시간이 흐름
    }
#endif

#if GPS_FAKE_DATA
    void GPS::generateFakeData() {
        unsigned long current_time = millis();
        bool updated = false;
        
        // 서울 위치로 설정 (약간의 랜덤 오차 추가)
        _latitude = _default_latitude + (random(-10, 10) / 10000.0);
        _longitude = _default_longitude + (random(-10, 10) / 10000.0);
        _altitude = _default_altitude + (random(-5, 5) / 10.0);
        
        // 위성 및 HDOP 정보 (랜덤 값)
        _satellites = random(8, 12);
        _hdop = 1.0 + (random(0, 20) / 100.0);
        
        // 위치 데이터는 매번 새로 생성
        _last_update_time = current_time;
        updated = true;
        
        // ESP32의 내부 시간 사용
        struct tm timeinfo;
        if (::getLocalTime(&timeinfo, 10)) {
            // ESP32 내부 시간을 사용 (기존 시간과 차이가 있을 때만 업데이트)
            int new_year = timeinfo.tm_year + 1900;
            int new_month = timeinfo.tm_mon + 1;
            int new_day = timeinfo.tm_mday;
            
            // 현재 시간은 이미 로컬 시간이므로 UTC로 변환
            int local_hour = timeinfo.tm_hour;
            int new_hour = local_hour - GPS_TIMEZONE_OFFSET;
            
            // 날짜 조정 (UTC로 변환 시 일자가 바뀔 수 있음)
            if (new_hour < 0) {
                new_hour += 24;
                // 전날로 조정 필요
                new_day--;
                if (new_day < 1) {
                    new_month--;
                    if (new_month < 1) {
                        new_month = 12;
                        new_year--;
                    }
                    // 월별 마지막 날짜 설정
                    switch(new_month) {
                        case 2: new_day = 28; break;  // 윤년 계산 생략
                        case 4: case 6: case 9: case 11: new_day = 30; break;
                        default: new_day = 31;
                    }
                }
            } else if (new_hour >= 24) {
                new_hour -= 24;
                // 다음날로 조정 필요
                new_day++;
                // 월별 날짜 체크는 생략 (복잡성 추가 방지)
            }
            
            int new_minute = timeinfo.tm_min;
            int new_second = timeinfo.tm_sec;
            
            // 시간이 변경되었는지 확인
            if (new_year != _utc_year || new_month != _utc_month || new_day != _utc_day ||
                new_hour != _utc_hour || new_minute != _utc_minute || new_second != _utc_second) {
                
                // 새 시간 적용
                _utc_year = new_year;
                _utc_month = new_month;
                _utc_day = new_day;
                _utc_hour = new_hour;
                _utc_minute = new_minute;
                _utc_second = new_second;
                
                // UTC 시간을 로컬 시간으로 변환
                convertUTCtoLocal();
                
                // 시간 데이터가 변경되었으므로 _last_gps_process_time 갱신
                _last_gps_process_time = current_time;
                
                // 시간 정보 업데이트 로그
                LOG(LOG_INFO, "ESP32 내부 시간 사용 (갱신됨): " + String(_utc_year) + "-" + 
                             String(_utc_month) + "-" + String(_utc_day) + " " + 
                             String(_utc_hour) + ":" + String(_utc_minute) + ":" + String(_utc_second) + " UTC");
                             
                updated = true;
            }
        } else {
            // ESP32 내부 시간을 가져올 수 없는 경우 millis() 기반 시간 생성
            unsigned long elapsed_sec = current_time / 1000;
            
            // 기본 날짜를 기준으로 경과된 시간을 계산
            unsigned long elapsed_days = elapsed_sec / 86400;
            unsigned long remaining_secs = elapsed_sec % 86400;
            
            // 기본 날짜를 기준점으로 설정
            int base_year = _default_year;
            int base_month = _default_month;
            int base_day = _default_day;
            
            // 간단한 날짜 계산 (모든 달이 30일로 계산)
            int days_in_month = 30;
            int days = base_day + elapsed_days;
            int months = base_month;
            int years = base_year;
            
            while (days > days_in_month) {
                days -= days_in_month;
                months++;
                if (months > 12) {
                    months = 1;
                    years++;
                }
            }
            
            // UTC 시간 설정
            _utc_year = years;
            _utc_month = months;
            _utc_day = days;
            
            // 하루 중 시간 계산 (UTC 기준)
            int total_seconds = _default_second + remaining_secs;
            int total_minutes = _default_minute + (total_seconds / 60);
            int total_hours = _default_hour + (total_minutes / 60);
            
            _utc_second = total_seconds % 60;
            _utc_minute = total_minutes % 60;
            _utc_hour = total_hours % 24;
            
            // UTC 시간을 로컬 시간으로 변환
            convertUTCtoLocal();
            
            LOG(LOG_INFO, "밀리초 카운터 기반 시간 생성: " + String(_utc_year) + "-" + 
                          String(_utc_month) + "-" + String(_utc_day) + " " + 
                          String(_utc_hour) + ":" + String(_utc_minute) + ":" + String(_utc_second) + " UTC");
                          
            updated = true;
        }
        
        if (updated) {
            LOG(LOG_INFO, "가짜 GPS 데이터 생성: " + String(_latitude, 6) + ", " + String(_longitude, 6) + 
                        ", UTC 시간: " + String(_utc_hour) + ":" + String(_utc_minute) + ":" + String(_utc_second) +
                        ", 로컬 시간: " + String(_local_hour) + ":" + String(_local_minute) + ":" + String(_local_second));
        }
    }
#endif

    // ===== 시간 변환 함수 =====
    void GPS::convertUTCtoLocal() {
        // UTC 시간 복사
        _local_year = _utc_year;
        _local_month = _utc_month;
        _local_day = _utc_day;
        
        // UTC에 timezone 오프셋을 더함
        int new_hour = _utc_hour + GPS_TIMEZONE_OFFSET;
        
        // 날짜 변경이 필요한 경우
        if (new_hour >= 24) {
            new_hour -= 24;
            
            // 날짜 증가
            _local_day++;
            
            // 월별 날짜 처리
            int days_in_month;
            
            // 월별 일수 (2월은 28일로 고정, 윤년 계산 생략)
            switch(_local_month) {
                case 2: days_in_month = 28; break;
                case 4: case 6: case 9: case 11: days_in_month = 30; break;
                default: days_in_month = 31;
            }
            
            // 월이 바뀌는 경우
            if (_local_day > days_in_month) {
                _local_day = 1;
                _local_month++;
                
                // 연도가 바뀌는 경우
                if (_local_month > 12) {
                    _local_month = 1;
                    _local_year++;
                }
            }
        } else if (new_hour < 0) {
            // 음수 시간 처리 (이전 날짜로)
            new_hour += 24;
            
            // 날짜 감소
            _local_day--;
            
            // 일이 0이 되면 이전 달로
            if (_local_day == 0) {
                _local_month--;
                
                // 월이 0이 되면 이전 연도로
                if (_local_month == 0) {
                    _local_month = 12;
                    _local_year--;
                }
                
                // 이전 달의 마지막 날짜 설정
                switch(_local_month) {
                    case 2: _local_day = 28; break;  // 윤년 처리 생략
                    case 4: case 6: case 9: case 11: _local_day = 30; break;
                    default: _local_day = 31;
                }
            }
        }
        
        // 변환된 시간 적용
        _local_hour = new_hour;
        _local_minute = _utc_minute;
        _local_second = _utc_second;
    }
    
    // ===== 시간 업데이트 함수 =====
    void GPS::updateCurrentTime(int &hour, int &minute, int &second) {
        unsigned long current_time = millis();
        unsigned long elapsed_ms = current_time - _last_gps_process_time;
        
        // 초, 분, 시간 계산
        unsigned long total_seconds = second + (elapsed_ms / 1000);
        unsigned long total_minutes = minute + (total_seconds / 60);
        unsigned long total_hours = hour + (total_minutes / 60);
        
        hour = total_hours % 24;
        minute = total_minutes % 60;
        second = total_seconds % 60;
    }
    
    void GPS::updateCurrentDateTime(int &year, int &month, int &day, int &hour, int &minute, int &second) {
        unsigned long current_time = millis();
        unsigned long elapsed_ms = current_time - _last_gps_process_time;
        
        // 초, 분, 시간 계산
        unsigned long total_seconds = second + (elapsed_ms / 1000);
        unsigned long total_minutes = minute + (total_seconds / 60);
        unsigned long total_hours = hour + (total_minutes / 60);
        
        // 일(day) 계산
        unsigned long elapsed_days = total_hours / 24;
        
        hour = total_hours % 24;
        minute = total_minutes % 60;
        second = total_seconds % 60;
        
        // 일 이상의 단위 변경이 필요한 경우
        if (elapsed_days > 0) {
            day += elapsed_days;
            
            // 월별 일수 확인
            int days_in_month;
            switch(month) {
                case 2: days_in_month = 28; break;  // 윤년 계산 생략
                case 4: case 6: case 9: case 11: days_in_month = 30; break;
                default: days_in_month = 31;
            }
            
            // 월이 바뀌는 경우 처리
            while (day > days_in_month) {
                day -= days_in_month;
                month++;
                
                // 연도가 바뀌는 경우
                if (month > 12) {
                    month = 1;
                    year++;
                }
                
                // 다음 달의 일수 계산
                switch(month) {
                    case 2: days_in_month = 28; break;  // 윤년 계산 생략
                    case 4: case 6: case 9: case 11: days_in_month = 30; break;
                    default: days_in_month = 31;
                }
            }
        }
    }
    
    // ===== 데이터 유효성 검사 및 기본값 설정 =====
    void GPS::validateData() {
        unsigned long current_time = millis();
        
        // 데이터가 오래된 경우 (30분 이상 경과)
        if (current_time - _last_update_time > GPS_DATA_TIMEOUT_MS) {
            LOG(LOG_WARN, "GPS 데이터 타임아웃: 기본값으로 설정");
            
            // 기본 위치로 설정
            _latitude = _default_latitude;
            _longitude = _default_longitude;
            _altitude = _default_altitude;
            
#if GPS_FAKE_DATA
            // GPS_FAKE_DATA 모드일 때 시간 업데이트
            struct tm timeinfo;
            if (::getLocalTime(&timeinfo, 10)) {
                // ESP32 내부 시간 사용
                _utc_year = timeinfo.tm_year + 1900;
                _utc_month = timeinfo.tm_mon + 1;
                _utc_day = timeinfo.tm_mday;
                
                // 현재 시간은 이미 로컬 시간이므로 UTC로 변환
                int local_hour = timeinfo.tm_hour;
                _utc_hour = local_hour - GPS_TIMEZONE_OFFSET;
                
                // 날짜 조정 (UTC로 변환 시 일자가 바뀔 수 있음)
                if (_utc_hour < 0) {
                    _utc_hour += 24;
                    // 전날로 조정
                    _utc_day--;
                    if (_utc_day < 1) {
                        _utc_month--;
                        if (_utc_month < 1) {
                            _utc_month = 12;
                            _utc_year--;
                        }
                        // 월별 마지막 날짜 설정
                        switch(_utc_month) {
                            case 2: _utc_day = 28; break;  // 윤년 계산 생략
                            case 4: case 6: case 9: case 11: _utc_day = 30; break;
                            default: _utc_day = 31;
                        }
                    }
                } else if (_utc_hour >= 24) {
                    _utc_hour -= 24;
                    // 다음날로 조정
                    _utc_day++;
                }
                
                _utc_minute = timeinfo.tm_min;
                _utc_second = timeinfo.tm_sec;
                
                // UTC 시간을 로컬 시간으로 변환
                convertUTCtoLocal();
            } else {
                // ESP32 내부 시간을 가져올 수 없는 경우, 마지막 시간 이후 경과된 시간으로 계산
                unsigned long elapsed_ms = current_time - _last_gps_process_time;
                
                // 시간 정보 업데이트 (이전 시간 기준으로 경과 시간 계산)
                int y = _utc_year, mo = _utc_month, d = _utc_day, h = _utc_hour, m = _utc_minute, s = _utc_second;
                
                // 경과 시간 반영
                updateCurrentDateTime(y, mo, d, h, m, s);
                
                // 업데이트된 값 적용
                _utc_year = y;
                _utc_month = mo;
                _utc_day = d;
                _utc_hour = h;
                _utc_minute = m;
                _utc_second = s;
                
                // UTC 시간을 로컬 시간으로 변환
                convertUTCtoLocal();
                
                LOG(LOG_INFO, "경과 시간 기반 시간 갱신: " + String(_utc_year) + "-" + 
                              String(_utc_month) + "-" + String(_utc_day) + " " + 
                              String(_utc_hour) + ":" + String(_utc_minute) + ":" + String(_utc_second) + " UTC");
            }
#else
            // 실제 GPS 모드일 때 경과 시간 계산으로 시간 업데이트
            unsigned long elapsed_ms = current_time - _last_gps_process_time;
            
            // 유효한 시간이 있는 경우만 업데이트
            if (_utc_year >= 2023) {
                // 시간 정보 업데이트 (이전 시간 기준으로 경과 시간 계산)
                int y = _utc_year, mo = _utc_month, d = _utc_day, h = _utc_hour, m = _utc_minute, s = _utc_second;
                
                // 경과 시간 반영
                updateCurrentDateTime(y, mo, d, h, m, s);
                
                // 업데이트된 값 적용
                _utc_year = y;
                _utc_month = mo;
                _utc_day = d;
                _utc_hour = h;
                _utc_minute = m;
                _utc_second = s;
                
                // UTC 시간을 로컬 시간으로 변환
                convertUTCtoLocal();
                
                LOG(LOG_INFO, "경과 시간 기반 시간 갱신: " + String(_utc_year) + "-" + 
                              String(_utc_month) + "-" + String(_utc_day) + " " + 
                              String(_utc_hour) + ":" + String(_utc_minute) + ":" + String(_utc_second) + " UTC");
            }
#endif
            
            // 위치 데이터가 기본값으로 설정되었음을 표시하기 위해 _last_update_time만 갱신
            _last_update_time = current_time;
            // _last_gps_process_time은 갱신하지 않음 - 시간이 계속 흐르게 함
        }
    }
    
    // ===== 시스템 시간 설정 함수 =====
    void GPS::setDefaultSystemTime() {
        struct tm tm;
        tm.tm_year = _default_year - 1900;  // 연도는 1900년부터 시작
        tm.tm_mon = _default_month - 1;     // 월은 0부터 시작
        tm.tm_mday = _default_day;
        tm.tm_hour = _default_hour + GPS_TIMEZONE_OFFSET;  // 로컬 시간으로 설정
        tm.tm_min = _default_minute;
        tm.tm_sec = _default_second;
        time_t t = mktime(&tm);
        struct timeval now = { .tv_sec = t };
        settimeofday(&now, NULL);
        
        LOG(LOG_INFO, "ESP32 시스템 시간을 기본값으로 설정: " + 
                      String(_default_year) + "-" + 
                      String(_default_month) + "-" + 
                      String(_default_day) + " " + 
                      String(_default_hour) + ":" + 
                      String(_default_minute) + ":" + 
                      String(_default_second) + 
                      " (UTC" + (GPS_TIMEZONE_OFFSET >= 0 ? "+" : "") + String(GPS_TIMEZONE_OFFSET) + ")");
    }

    // ===== 위치 데이터 접근 함수 =====
    double GPS::getLatitude() {
        return _latitude;
    }
    
    double GPS::getLongitude() {
        return _longitude;
    }
    
    double GPS::getAltitude() {
        return _altitude;
    }
    
    int GPS::getSatellites() {
        return _satellites;
    }
    
    float GPS::getHDOP() {
        return _hdop;
    }
    
    void GPS::getLocation(double &lat, double &lng, double &alt) {
        lat = _latitude;
        lng = _longitude;
        alt = _altitude;
    }
    
    String GPS::getFormattedLocation() {
        char location_str[50];
        sprintf(location_str, "%.6f, %.6f, %.2fm", _latitude, _longitude, _altitude);
        return String(location_str);
    }

    // ===== UTC 시간 관련 함수 =====
    void GPS::getUTCDate(int &year, int &month, int &day) {
        // 현재 날짜/시간 전체를 업데이트
        int h = _utc_hour, m = _utc_minute, s = _utc_second;
        year = _utc_year;
        month = _utc_month;
        day = _utc_day;
        
        // 경과 시간을 반영하여 날짜/시간 모두 갱신
        updateCurrentDateTime(year, month, day, h, m, s);
    }
    
    void GPS::getUTCTime(int &hour, int &minute, int &second) {
        // 전체 날짜/시간을 함께 업데이트
        int y = _utc_year, mo = _utc_month, d = _utc_day;
        hour = _utc_hour;
        minute = _utc_minute;
        second = _utc_second;
        
        // 경과 시간을 반영하여 날짜/시간 모두 갱신
        updateCurrentDateTime(y, mo, d, hour, minute, second);
    }
    
    String GPS::getFormattedUTCDate() {
        int y, mo, d;
        getUTCDate(y, mo, d);
        
        char date_str[11];
        sprintf(date_str, "%04d-%02d-%02d", y, mo, d);
        return String(date_str);
    }
    
    String GPS::getFormattedUTCTime() {
        int h, m, s;
        getUTCTime(h, m, s);
        
        char time_str[9];
        sprintf(time_str, "%02d:%02d:%02d", h, m, s);
        return String(time_str);
    }
    
    String GPS::getFormattedUTCDateTime() {
        int y, mo, d, h, m, s;
        getUTCDate(y, mo, d);
        getUTCTime(h, m, s);
        
        char datetime_str[20];
        sprintf(datetime_str, "%04d-%02d-%02d %02d:%02d:%02d", y, mo, d, h, m, s);
        return String(datetime_str) + " UTC";
    }

    // ===== 로컬 시간 관련 함수 =====
    void GPS::getLocalDate(int &year, int &month, int &day) {
        // 현재 날짜/시간 전체를 업데이트
        int h = _local_hour, m = _local_minute, s = _local_second;
        year = _local_year;
        month = _local_month;
        day = _local_day;
        
        // 경과 시간을 반영하여 날짜/시간 모두 갱신
        updateCurrentDateTime(year, month, day, h, m, s);
    }
    
    void GPS::getLocalTime(int &hour, int &minute, int &second) {
        // 전체 날짜/시간을 함께 업데이트
        int y = _local_year, mo = _local_month, d = _local_day;
        hour = _local_hour;
        minute = _local_minute;
        second = _local_second;
        
        // 경과 시간을 반영하여 날짜/시간 모두 갱신
        updateCurrentDateTime(y, mo, d, hour, minute, second);
    }
    
    String GPS::getFormattedLocalDate() {
        int y, mo, d;
        getLocalDate(y, mo, d);
        
        char date_str[11];
        sprintf(date_str, "%04d-%02d-%02d", y, mo, d);
        return String(date_str);
    }
    
    String GPS::getFormattedLocalTime() {
        int h, m, s;
        getLocalTime(h, m, s);
        
        char time_str[9];
        sprintf(time_str, "%02d:%02d:%02d", h, m, s);
        return String(time_str);
    }
    
    String GPS::getFormattedLocalDateTime() {
        int y, mo, d, h, m, s;
        getLocalDate(y, mo, d);
        getLocalTime(h, m, s);
        
        char datetime_str[20];
        sprintf(datetime_str, "%04d-%02d-%02d %02d:%02d:%02d", y, mo, d, h, m, s);
        return String(datetime_str) + " (UTC" + (GPS_TIMEZONE_OFFSET >= 0 ? "+" : "") + String(GPS_TIMEZONE_OFFSET) + ")";
    }
    
    // ===== 이전 버전 호환용 함수 =====
    void GPS::getDate(int &year, int &month, int &day) {
        getLocalDate(year, month, day);
    }
    
    void GPS::getTime(int &hour, int &minute, int &second) {
        getLocalTime(hour, minute, second);
    }
    
    String GPS::getFormattedDate() {
        return getFormattedLocalDate();
    }
    
    String GPS::getFormattedTime() {
        return getFormattedLocalTime();
    }

    // ===== 기본값 설정 함수 =====
    void GPS::setDefaultLocation(double latitude, double longitude, double altitude) {
        _default_latitude = latitude;
        _default_longitude = longitude;
        _default_altitude = altitude;
        
        LOG(LOG_INFO, "GPS 기본 위치 설정: " + String(latitude, 6) + ", " + 
                       String(longitude, 6) + ", " + String(altitude, 2) + "m");
    }
    
    void GPS::setDefaultDate(int year, int month, int day) {
        // 날짜 유효성 검사
        if (year < 2023 || month < 1 || month > 12 || day < 1 || day > 31) {
            LOG(LOG_ERROR, "GPS 기본 날짜 설정 실패: 유효하지 않은 날짜 (년월일: " + 
                            String(year) + "-" + String(month) + "-" + String(day) + ")");
            return;
        }
        
        _default_year = year;
        _default_month = month;
        _default_day = day;
        
        LOG(LOG_INFO, "GPS 기본 날짜 설정: " + String(year) + "-" + 
                       String(month) + "-" + String(day));
    }
    
    void GPS::setDefaultTime(int hour, int minute, int second) {
        // 시간 유효성 검사
        if (hour < 0 || hour > 23 || minute < 0 || minute > 59 || second < 0 || second > 59) {
            LOG(LOG_ERROR, "GPS 기본 시간 설정 실패: 유효하지 않은 시간 (시분초: " + 
                            String(hour) + ":" + String(minute) + ":" + String(second) + ")");
            return;
        }
        
        _default_hour = hour;
        _default_minute = minute;
        _default_second = second;
        
        LOG(LOG_INFO, "GPS 기본 시간 설정: " + String(hour) + ":" + 
                       String(minute) + ":" + String(second) + " UTC");
    }
    
    // ===== 기본값 조회 함수 =====
    void GPS::getDefaultLocation(double &latitude, double &longitude, double &altitude) {
        latitude = _default_latitude;
        longitude = _default_longitude;
        altitude = _default_altitude;
    }
    
    void GPS::getDefaultDate(int &year, int &month, int &day) {
        year = _default_year;
        month = _default_month;
        day = _default_day;
    }
    
    void GPS::getDefaultTime(int &hour, int &minute, int &second) {
        hour = _default_hour;
        minute = _default_minute;
        second = _default_second;
    }

    // ===== 수동 설정 함수 =====
    void GPS::setCurrentLocation(double latitude, double longitude, double altitude) {
        _latitude = latitude;
        _longitude = longitude;
        _altitude = altitude;
        
        // 마지막 업데이트 시간 갱신
        _last_update_time = millis();
        
        LOG(LOG_INFO, "GPS 현재 위치 수동 설정: " + String(latitude, 6) + ", " + 
                       String(longitude, 6) + ", " + String(altitude, 2) + "m");
    }
    
    void GPS::setCurrentUTCTime(int year, int month, int day, int hour, int minute, int second) {
        // 날짜/시간 유효성 검사
        if (year < 2023 || month < 1 || month > 12 || day < 1 || day > 31 ||
            hour < 0 || hour > 23 || minute < 0 || minute > 59 || second < 0 || second > 59) {
            LOG(LOG_ERROR, "GPS 현재 시간 설정 실패: 유효하지 않은 날짜/시간");
            return;
        }
        
        // UTC 시간 설정
        _utc_year = year;
        _utc_month = month;
        _utc_day = day;
        _utc_hour = hour;
        _utc_minute = minute;
        _utc_second = second;
        
        // UTC 시간을 로컬 시간으로 변환
        convertUTCtoLocal();
        
        // ESP32 시스템 시간도 함께 설정 (UTC 기준)
        struct tm tm;
        tm.tm_year = year - 1900;  // 연도는 1900년부터 시작
        tm.tm_mon = month - 1;     // 월은 0부터 시작
        tm.tm_mday = day;
        tm.tm_hour = hour;
        tm.tm_min = minute;
        tm.tm_sec = second;
        time_t t = mktime(&tm);
        struct timeval now = { .tv_sec = t };
        settimeofday(&now, NULL);
        
        // 마지막 업데이트 시간 및 처리 시간 갱신
        _last_update_time = millis();
        _last_gps_process_time = millis();
        
        LOG(LOG_INFO, "GPS 현재 시간 수동 설정 (UTC): " + String(year) + "-" + 
                      String(month) + "-" + String(day) + " " + String(hour) + ":" + 
                      String(minute) + ":" + String(second) + 
                      ", 로컬 시간: " + String(_local_hour) + ":" + 
                      String(_local_minute) + ":" + String(_local_second));
    }
} 