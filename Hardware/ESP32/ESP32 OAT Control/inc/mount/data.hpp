#pragma once

#include <Arduino.h>

// Mount(OAT)의 모든 데이터를 관리하는 곳, 버전이나 이런 것들을 설정하거나 가져올 수 있게 하는 것들

namespace Mount {
    class Data {
    private:
        // Info
        String _info_product_name = "Unknown";
        String _info_firmware_version = "Unknown";
        int _info_firmware_version_num = 0;

        // Info - state
        String _info_state_board = "Unknown";
        String _info_state_ra_stepper = "Unknown";
        String _info_state_dec_stepper = "Unknown";
        String _info_state_gps = "Unknown";
        String _info_state_azalt_stepper = "Unknown";
        String _info_state_gyro = "Unknown";
        String _info_state_display = "Unknown";
        String _info_state_focuser = "Unknown";
        String _info_state_ra_hall_sensor = "Unknown";
        String _info_state_dec_hall_sensor = "Unknown";
        String _info_state_endswitch_sensor = "Unknown";
        
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

        Data() {}
        ~Data() {}

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
        
        // Get - Stepper info
        static String get_stepper_ra_driver();
        static String get_stepper_dec_driver();
        static String get_stepper_ra_slew_ms();
        static String get_stepper_ra_track_ms();
        static String get_stepper_dec_slew_ms();
        static String get_stepper_dec_guide_ms();
        
        // Feature flags
        static bool has_gps();
        static bool has_alt();
        static bool has_az();
        static bool has_gyro();
        static bool has_focuser();
        static bool has_ra_hall_sensor();
        static bool has_dec_hall_sensor();

        // Set
        static void set_info_product_name(String product_name);
        static void set_info_firmware_version(String firmware_version);
        static void set_info_state(String state);
        static void set_stepper_info(String stepper_info);
    };
}