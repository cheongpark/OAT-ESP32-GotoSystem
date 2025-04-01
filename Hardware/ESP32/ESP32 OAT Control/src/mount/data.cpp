#include "mount/data.hpp"

namespace Mount {
    Data& Data::getInstance() {
        static Data instance;
        return instance;
    }
    
    // Get - 기본 정보
    String Data::get_info_product_name() {
        return getInstance()._info_product_name;
    }
    
    String Data::get_info_firmware_version() {
        return getInstance()._info_firmware_version;
    }

    int Data::get_info_firmware_version_num() {
        return getInstance()._info_firmware_version_num;
    }

    String Data::get_info_state_board() {
        return getInstance()._info_state_board;
    }

    String Data::get_info_state_ra_stepper() {
        return getInstance()._info_state_ra_stepper;
    }

    String Data::get_info_state_dec_stepper() {
        return getInstance()._info_state_dec_stepper;
    }

    String Data::get_info_state_display() {
        return getInstance()._info_state_display;
    }

    bool Data::get_connected() {
        return getInstance()._connected;
    }

    // Get - 스테퍼 정보
    String Data::get_stepper_ra_driver() {
        return getInstance()._info_state_stepper_ra_driver;
    }

    String Data::get_stepper_dec_driver() {
        return getInstance()._info_state_stepper_dec_driver;
    }

    String Data::get_stepper_ra_slew_ms() {
        return getInstance()._info_state_stepper_ra_slew_ms;
    }

    String Data::get_stepper_ra_track_ms() {
        return getInstance()._info_state_stepper_ra_track_ms;
    }

    String Data::get_stepper_dec_slew_ms() {
        return getInstance()._info_state_stepper_dec_slew_ms;
    }

    String Data::get_stepper_dec_guide_ms() {
        return getInstance()._info_state_stepper_dec_guide_ms;
    }

    // Get - 기능 플래그
    bool Data::has_gps() {
        return getInstance()._has_gps;
    }

    bool Data::has_alt() {
        return getInstance()._has_alt;
    }

    bool Data::has_az() {
        return getInstance()._has_az;
    }

    bool Data::has_gyro() {
        return getInstance()._has_gyro;
    }

    bool Data::has_focuser() {
        return getInstance()._has_focuser;
    }

    bool Data::has_ra_hall_sensor() {
        return getInstance()._has_ra_hall_sensor;
    }

    bool Data::has_dec_hall_sensor() {
        return getInstance()._has_dec_hall_sensor;
    }

    float Data::get_ra_dec_slew_rate() {
        return getInstance()._ra_dec_slew_rate;
    }

    float Data::get_az_alt_slew_rate() {
        return getInstance()._az_alt_slew_rate;
    }

    float Data::get_steps_ra() {
        return getInstance()._steps_ra;
    }

    float Data::get_steps_dec() {
        return getInstance()._steps_dec;
    }

    float Data::get_speed_factor() {
        return getInstance()._speed_factor;
    }
    
    String Data::get_lst() {
        return getInstance()._lst;
    }

    float Data::get_longitude() {
        return getInstance()._longitude;
    }

    float Data::get_latitude() {
        return getInstance()._latitude;
    }
    

    // Set
    void Data::set_info_product_name(String product_name) {
        getInstance()._info_product_name = product_name;
    }
    
    void Data::set_info_firmware_version(String firmware_version) {
        Data& instance = getInstance();
        instance._info_firmware_version = firmware_version;
        
        // 버전 형식 "V1.13.13"을 숫자로 변환 (10000*major + 100*minor + patch)
        String version_str = firmware_version;
        int version_num = 0;
        
        // V 제거
        if (version_str.startsWith("V")) {
            version_str = version_str.substring(1);
        }
        
        // 버전 분리
        int first_dot = version_str.indexOf('.');
        if (first_dot > 0) {
            int second_dot = version_str.indexOf('.', first_dot + 1);
            if (second_dot > 0) {
                int major = version_str.substring(0, first_dot).toInt();
                int minor = version_str.substring(first_dot + 1, second_dot).toInt();
                int patch = version_str.substring(second_dot + 1).toInt();
                
                version_num = major * 10000 + minor * 100 + patch;
            }
        }
        
        instance._info_firmware_version_num = version_num;
    }

    void Data::set_info_state(String state) {
        Data& instance = getInstance();
        
        // 상태 정보 파싱 (XGM 명령의 응답)
        // 형식: board,ra_stepper|steps,dec_stepper|steps,[addons...]
        int comma_pos = state.indexOf(',');
        if (comma_pos < 0) return;
        
        // 보드 정보
        instance._info_state_board = state.substring(0, comma_pos);
        
        // RA 스테퍼 정보
        int next_comma_pos = state.indexOf(',', comma_pos + 1);
        if (next_comma_pos < 0) return;
        
        String ra_part = state.substring(comma_pos + 1, next_comma_pos);
        int ra_separator_pos = ra_part.indexOf('|');
        if (ra_separator_pos >= 0) {
            instance._info_state_ra_stepper = ra_part.substring(0, ra_separator_pos) + ", " + ra_part.substring(ra_separator_pos + 1) + "T";
        }
        
        // DEC 스테퍼 정보
        comma_pos = next_comma_pos;
        next_comma_pos = state.indexOf(',', comma_pos + 1);
        String dec_part;
        
        if (next_comma_pos < 0) {
            dec_part = state.substring(comma_pos + 1);
        } else {
            dec_part = state.substring(comma_pos + 1, next_comma_pos);
        }
        
        int dec_separator_pos = dec_part.indexOf('|');
        if (dec_separator_pos >= 0) {
            instance._info_state_dec_stepper = dec_part.substring(0, dec_separator_pos) + ", " + dec_part.substring(dec_separator_pos + 1) + "T";
        }
        
        // 애드온 정보
        instance._has_gps = false;
        instance._has_alt = false;
        instance._has_az = false;
        instance._has_gyro = false;
        instance._has_focuser = false;
        instance._has_ra_hall_sensor = false;
        instance._has_dec_hall_sensor = false;
        instance._info_state_display = "None";
        
        if (next_comma_pos > 0) {
            String addons = state.substring(next_comma_pos + 1);
            while (addons.length() > 0) {
                int addon_comma_pos = addons.indexOf(',');
                String addon;
                
                if (addon_comma_pos < 0) {
                    addon = addons;
                    addons = "";
                } else {
                    addon = addons.substring(0, addon_comma_pos);
                    addons = addons.substring(addon_comma_pos + 1);
                }
                
                if (addon == "AUTO_AZ_ALT") {
                    instance._has_alt = true;
                    instance._has_az = true;
                } else if (addon == "AUTO_ALT") {
                    instance._has_alt = true;
                } else if (addon == "AUTO_AZ") {
                    instance._has_az = true;
                } else if (addon == "LCD_KEYPAD") {
                    instance._info_state_display = "16x2 LCD (w/ buttons)";
                } else if (addon == "LCD_I2C_MCP23008") {
                    instance._info_state_display = "LCD (MCP23008 on I2C)";
                } else if (addon == "LCD_I2C_MCP23017") {
                    instance._info_state_display = "LCD (MCP23017 on I2C)";
                } else if (addon == "LCD_JOY_I2C_SSD1306") {
                    instance._info_state_display = "Pixel OLED (SSD1306 on I2C) w/ joystick";
                } else if (addon == "FOC") {
                    instance._has_focuser = true;
                } else if (addon == "HSAH") {
                    instance._has_ra_hall_sensor = true;
                } else if (addon == "HSAV") {
                    instance._has_dec_hall_sensor = true;
                } else if (addon.startsWith("INFO_")) {
                    int underscore_pos_1 = addon.indexOf('_', 5);
                    int underscore_pos_2 = addon.indexOf('_', underscore_pos_1 + 1);
                    
                    if (underscore_pos_1 > 0 && underscore_pos_2 > 0) {
                        String display_type = addon.substring(underscore_pos_2 + 1);
                        String protocol = addon.substring(underscore_pos_1 + 1, underscore_pos_2);
                        String bus = addon.substring(5, underscore_pos_1);
                        
                        instance._info_state_display = display_type + " Info Display (" + protocol + " on " + bus + ")";
                    } else {
                        instance._info_state_display = "Info Display";
                    }
                }
            }
        }
    }
    
    void Data::set_stepper_info(String stepper_info) {
        Data& instance = getInstance();
        
        // 스테퍼 정보 파싱 (XGMS 명령의 응답)
        // 형식: ra_driver,ra_slew_ms,ra_track_ms|dec_driver,dec_slew_ms,dec_guide_ms
        int separator_pos = stepper_info.indexOf('|');
        if (separator_pos < 0) return;
        
        // RA 스테퍼 정보
        String ra_part = stepper_info.substring(0, separator_pos);
        int comma_pos_1 = ra_part.indexOf(',');
        if (comma_pos_1 < 0) return;
        
        int comma_pos_2 = ra_part.indexOf(',', comma_pos_1 + 1);
        if (comma_pos_2 < 0) return;
        
        String ra_driver = ra_part.substring(0, comma_pos_1);
        instance._info_state_stepper_ra_slew_ms = ra_part.substring(comma_pos_1 + 1, comma_pos_2);
        instance._info_state_stepper_ra_track_ms = ra_part.substring(comma_pos_2 + 1);
        
        // RA 드라이버 타입 변환
        if (ra_driver == "U") {
            instance._info_state_stepper_ra_driver = "ULN2003";
        } else if (ra_driver == "TU") {
            instance._info_state_stepper_ra_driver = "TMC2209 UART";
        } else if (ra_driver == "TS") {
            instance._info_state_stepper_ra_driver = "TMC2209 Standalone";
        } else if (ra_driver == "A") {
            instance._info_state_stepper_ra_driver = "A4988 Generic";
        } else {
            instance._info_state_stepper_ra_driver = ra_driver;
        }
        
        // DEC 스테퍼 정보
        String dec_part = stepper_info.substring(separator_pos + 1);
        // 마지막 | 문자 제거
        if (dec_part.endsWith("|")) {
            dec_part = dec_part.substring(0, dec_part.length() - 1);
        }
        
        comma_pos_1 = dec_part.indexOf(',');
        if (comma_pos_1 < 0) return;
        
        comma_pos_2 = dec_part.indexOf(',', comma_pos_1 + 1);
        if (comma_pos_2 < 0) return;
        
        String dec_driver = dec_part.substring(0, comma_pos_1);
        instance._info_state_stepper_dec_slew_ms = dec_part.substring(comma_pos_1 + 1, comma_pos_2);
        instance._info_state_stepper_dec_guide_ms = dec_part.substring(comma_pos_2 + 1);
        
        // DEC 드라이버 타입 변환
        if (dec_driver == "U") {
            instance._info_state_stepper_dec_driver = "ULN2003";
        } else if (dec_driver == "TU") {
            instance._info_state_stepper_dec_driver = "TMC2209 UART";
        } else if (dec_driver == "TS") {
            instance._info_state_stepper_dec_driver = "TMC2209 Standalone";
        } else if (dec_driver == "A") {
            instance._info_state_stepper_dec_driver = "A4988 Generic";
        } else {
            instance._info_state_stepper_dec_driver = dec_driver;
        }
    }
    
    void Data::set_connected(bool connected) {
        getInstance()._connected = connected;
    }
    
    /*
    slew_rates_num: [0, 1, 2, 3, 4, 5]
    */
    void Data::set_ra_dec_slew_rate(int slew_rates_num) {
        if (slew_rates_num < 0 || slew_rates_num > 5) 
            return;

        getInstance()._ra_dec_slew_rate = OAT_RADEC_SLEW_RATES[slew_rates_num];
    }

    void Data::set_az_alt_slew_rate(int slew_rates_num) {
        if (slew_rates_num < 0 || slew_rates_num > 5) 
            return;
            
        getInstance()._az_alt_slew_rate = OAT_AZALT_SLEW_RATES[slew_rates_num];
    }

    void Data::set_steps_ra(String steps) {
        getInstance()._steps_ra = max(1.0f, steps.toFloat());
    }

    void Data::set_steps_dec(String steps) {
        getInstance()._steps_dec = max(1.0f, steps.toFloat());
    }

    void Data::set_speed_factor(String speed_factor) {
        getInstance()._speed_factor = speed_factor.toFloat();
    }

    void Data::set_lst(String lst) {
        if (lst.length() == 6) {
            lst = lst.substring(0, 2) + ":" + lst.substring(2, 4) + ":" + lst.substring(4);
        }

        getInstance()._lst = lst;
    }

    void Data::set_longitude(String lon) {
        double longitude = 0.0;
        int values[3] = {0, 0, 0};
        int valueIndex = 0;
        int startPos = 0;
        
        // 문자열 파싱
        for (int i = 0; i < lon.length(); i++) {
            // 구분자 찾기 ('*' 또는 '\'')
            if (lon.charAt(i) == '*' || lon.charAt(i) == '\'') {
                // 현재 부분을 정수로 변환
                values[valueIndex++] = lon.substring(startPos, i).toInt();
                startPos = i + 1;
                
                // 최대 3개의 부분만 처리
                if (valueIndex >= 3) break;
            }
        }
        
        // 마지막 부분 처리 (구분자 이후)
        if (startPos < lon.length() && valueIndex < 3) {
            values[valueIndex] = lon.substring(startPos).toInt();
        }
        
        // 도, 분, 초를 10진수로 변환
        longitude = values[0] + values[1] / 60.0;
        if (valueIndex >= 2) {
            longitude += values[2] / 3600.0;
        }

        if (getInstance()._info_firmware_version_num < 11105) {
            longitude = 180 - longitude;
        } else {
            longitude = -longitude;
        }

        getInstance()._longitude = longitude;
    }

    void Data::set_latitude(String lat) {
        double latitude = 0.0;
        int values[3] = {0, 0, 0};
        int valueIndex = 0;
        int startPos = 0;
        
        // 문자열 파싱
        for (int i = 0; i < lat.length(); i++) {
            // 구분자 찾기 ('*' 또는 '\'')
            if (lat.charAt(i) == '*' || lat.charAt(i) == '\'') {
                // 현재 부분을 정수로 변환
                values[valueIndex++] = lat.substring(startPos, i).toInt();
                startPos = i + 1;
                
                // 최대 3개의 부분만 처리
                if (valueIndex >= 3) break;
            }
        }
        
        // 마지막 부분 처리 (구분자 이후)
        if (startPos < lat.length() && valueIndex < 3) {
            values[valueIndex] = lat.substring(startPos).toInt();
        }
        
        // 도, 분, 초를 10진수로 변환
        latitude = values[0] + values[1] / 60.0;
        if (valueIndex >= 2) {
            latitude += values[2] / 3600.0;
        }

        getInstance()._latitude = latitude;
    }

    // 상태 업데이트

    void Data::updateStatus() {
        if (!getInstance()._connected) return;
        if (!shouldUpdateStatus()) return;
        
        // OAT 상태 요청 (GX)
        Response response = getInstance().mount_serial.request("GX");
        if (!response.success) return;

        // 상태 정보 파싱
        String status = response.message;
        int comma_pos = status.indexOf(',');
        if (comma_pos < 0) return;

        // 상태 정보 추출
        String mount_status = status.substring(0, comma_pos);
        String ra_position = "";
        String dec_position = "";
        String trk_position = "";
        String focus_position = "";
        String ra_coordinate = "";
        String dec_coordinate = "";

        // RA/DEC 위치 정보 추출
        int next_comma = status.indexOf(',', comma_pos + 1);
        if (next_comma > 0) {
            ra_position = status.substring(comma_pos + 1, next_comma);
            comma_pos = next_comma;
        }

        next_comma = status.indexOf(',', comma_pos + 1);
        if (next_comma > 0) {
            dec_position = status.substring(comma_pos + 1, next_comma);
            comma_pos = next_comma;
        }

        next_comma = status.indexOf(',', comma_pos + 1);
        if (next_comma > 0) {
            trk_position = status.substring(comma_pos + 1, next_comma);
            comma_pos = next_comma;
        }

        // RA/DEC 좌표 정보 추출
        next_comma = status.indexOf(',', comma_pos + 1);
        if (next_comma > 0) {
            ra_coordinate = status.substring(comma_pos + 1, next_comma);
            comma_pos = next_comma;
        }

        next_comma = status.indexOf(',', comma_pos + 1);
        if (next_comma > 0) {
            dec_coordinate = status.substring(comma_pos + 1, next_comma);
            comma_pos = next_comma;
        }

        // Focus 정보 추출 (있는 경우)
        if (comma_pos + 1 < status.length()) {
            focus_position = status.substring(comma_pos + 1);
        }

        // LST 요청
        response = getInstance().mount_serial.request("XGL");
        if (response.success) {
            String lst = response.message;
            if (lst.length() == 6) {
                lst = lst.substring(0, 2) + ":" + lst.substring(2, 4) + ":" + lst.substring(4);
            }
            set_lst(lst);
        }

        String status_message = "상태 업데이트 - 위치: " + ra_position + ", " + dec_position + ", " + String(getInstance()._last_status_update);
        LOG(LOG_INFO, status_message);

        // 상태 정보 업데이트
        set_info_state(mount_status);
        set_steps_ra(ra_position);
        set_steps_dec(dec_position);
        set_speed_factor(trk_position);

        // 마지막 업데이트 시간 갱신
        getInstance()._last_status_update = millis();
    }

    bool Data::shouldUpdateStatus() {
        unsigned long current_time = millis();
        unsigned long elapsed = current_time - getInstance()._last_status_update;
        return elapsed >= DATA_UPDATE_INTERVAL_MS;
    }
}