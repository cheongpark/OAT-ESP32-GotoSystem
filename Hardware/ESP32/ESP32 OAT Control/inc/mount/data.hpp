#pragma once

#include <Arduino.h>

#include "settings.hpp"

// Mount(OAT)의 모든 데이터를 관리하는 곳, 버전이나 이런 것들을 설정하거나 가져올 수 있게 하는 것들

namespace Mount {
    class Data {
    private:
        // Info
        String _info_product_name = "Unknown";
        String _info_firmware_version = "Unknown";
        int _info_firmware_version_num = 0;
        bool _connected = false;

        // Info - state
        String _info_state_board = "Unknown";
        String _info_state_ra_stepper = "Unknown";
        String _info_state_dec_stepper = "Unknown";
        String _info_state_display = "Unknown";
        
        // Stepper info
        String _info_state_stepper_ra_driver = "Unknown";
        String _info_state_stepper_dec_driver = "Unknown";
        String _info_state_stepper_ra_slew_ms = "Unknown";
        String _info_state_stepper_ra_track_ms = "Unknown";
        String _info_state_stepper_dec_slew_ms = "Unknown";
        String _info_state_stepper_dec_guide_ms = "Unknown";

        // Feature flags
        bool _has_gps = false;
        bool _has_alt = false;
        bool _has_az = false;
        bool _has_gyro = false;
        bool _has_focuser = false;
        bool _has_ra_hall_sensor = false;
        bool _has_dec_hall_sensor = false;

        // TODO az_alt는 변수에서 변경되게 하고.. ra_dec은 OAT에 요청해서 값을 넣거나 가져와서 변경하도록 해야함
        float _ra_dec_slew_rate = OAT_RADEC_SLEW_RATES[5]; // 기본 속도
        float _az_alt_slew_rate = OAT_AZALT_SLEW_RATES[5]; // 기본 속도
        float _steps_ra = 0;
        float _steps_dec = 0;
        float _speed_factor = 0;
        String _lst = "00:00:00";
        float _longitude = 0;
        float _latitude = 0;
                
        Data() {}

    public:
        // Singletone
        Data(const Data&) = delete;
        Data& operator=(const Data&) = delete;

        static Data& getInstance();

        // Get
        static String get_info_product_name();
        static String get_info_firmware_version();
        static int get_info_firmware_version_num();
        static String get_info_state_board();
        static String get_info_state_ra_stepper();
        static String get_info_state_dec_stepper();
        static String get_info_state_display();
        static bool get_connected();
        
        // Get - Stepper info
        static String get_stepper_ra_driver();
        static String get_stepper_dec_driver();
        static String get_stepper_ra_slew_ms();
        static String get_stepper_ra_track_ms();
        static String get_stepper_dec_slew_ms();
        static String get_stepper_dec_guide_ms();

        static float get_steps_ra();
        static float get_steps_dec();
        static float get_speed_factor();

        static String get_lst();
        static float get_longitude();
        static float get_latitude();

        // Feature flags
        static bool has_gps();
        static bool has_alt();
        static bool has_az();
        static bool has_gyro();
        static bool has_focuser();
        static bool has_ra_hall_sensor();
        static bool has_dec_hall_sensor();

        static float get_ra_dec_slew_rate();
        static float get_az_alt_slew_rate();

        // Set
        static void set_info_product_name(String product_name);
        static void set_info_firmware_version(String firmware_version);
        static void set_info_state(String state);
        static void set_stepper_info(String stepper_info);
        static void set_connected(bool connected);

        static void set_ra_dec_slew_rate(int slew_rates_num);
        static void set_az_alt_slew_rate(int slew_rates_num);
        static void set_steps_ra(String steps);
        static void set_steps_dec(String steps);
        static void set_speed_factor(String speed_factor);

        static void set_lst(String lst);
        static void set_longitude(String longitude);
        static void set_latitude(String latitude);
    };
}