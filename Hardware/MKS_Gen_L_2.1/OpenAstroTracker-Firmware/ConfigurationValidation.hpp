#pragma once

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                            ////////
// 설정 검증                   ////////
//                            ////////
//////////////////////////////////////

#define MIN_CONFIG_VERSION 1

#ifndef CONFIG_VERSION
    #define CONFIG_VERSION 1
#endif

#if CONFIG_VERSION < MIN_CONFIG_VERSION
    #error This firmware requires more recent configuration, visit https://config.openastrotech.com/
#endif

// 플랫폼
#if defined(ESP32) || defined(__AVR_ATmega2560__)
// 유효한 플랫폼
#else
    #error Unsupported platform configuration. Use at own risk.
#endif

// 디스플레이 & 키패드 설정
#if defined(ESP32) && ((DISPLAY_TYPE == DISPLAY_TYPE_NONE) || (DISPLAY_TYPE == DISPLAY_TYPE_LCD_JOY_I2C_SSD1306))
// ESP32용 유효한 디스플레이
#elif defined(__AVR_ATmega2560__)                                                                                                          \
    && ((DISPLAY_TYPE == DISPLAY_TYPE_NONE) || (DISPLAY_TYPE == DISPLAY_TYPE_LCD_KEYPAD) || (DISPLAY_TYPE_LCD_KEYPAD_I2C_MCP23008)         \
        || (DISPLAY_TYPE_LCD_KEYPAD_I2C_MCP23017))
// ATmega용 유효한 디스플레이
#else
    #error Unsupported display configuration. Use at own risk.
#endif

// 정보 디스플레이 검증
#if defined(__AVR_ATmega2560__)
    #if (INFO_DISPLAY_TYPE != INFO_DISPLAY_TYPE_NONE)
        #if (DISPLAY_TYPE != DISPLAY_TYPE_NONE)
            #error Unsupported configuration. Not enough memory to use LCD and OLED at the same time.
        #endif
        #if (FOCUS_STEPPER_TYPE != STEPPER_TYPE_NONE)
            #if (BOARD == BOARD_AVR_MKS_GEN_L_V1) || (BOARD == BOARD_AVR_MKS_GEN_L_V2) || (BOARD == BOARD_AVR_MKS_GEN_L_V21)
                #warning OLED requires I2C, so E1 port on a MKS GenL is not available. Make sure you are not attempting to use it.
            #endif
        #endif
    #endif
#endif

#if (RA_DRIVER_TYPE == DRIVER_TYPE_TMC2209_UART)
    #ifndef RA_DRIVER_ADDRESS
        // UART 모드의 TMC2209에는 시리얼 버스 주소가 지정되어야 함
        #error RA driver address for DRIVER_TYPE_TMC2209_UART not specified.
    #endif
    #if (RA_UART_STEALTH_MODE != 0) && (RA_UART_STEALTH_MODE != 1)
        // 스텔스 모드는 0 또는 1이어야 함
        #error RA stealth mode must be 0 (off) or 1 (on)
    #endif
#endif

#if (DEC_DRIVER_TYPE == DRIVER_TYPE_TMC2209_UART)
    #ifndef DEC_DRIVER_ADDRESS
        // UART 모드의 TMC2209에는 시리얼 버스 주소가 지정되어야 함
        #error DEC driver address for DRIVER_TYPE_TMC2209_UART not specified.
    #endif
    #if (DEC_UART_STEALTH_MODE != 0) && (DEC_UART_STEALTH_MODE != 1)
        // 스텔스 모드는 0 또는 1이어야 함
        #error DEC stealth mode must be 0 (off) or 1 (on)
    #endif
#endif

#ifdef AZIMUTH_ALTITUDE_MOTORS
    #error Configuration out of date! Please remove AZIMUTH_ALTITUDE_MOTORS and replace with AZ_STEPPER_TYPE, AZ_DRIVER_TYPE and ALT_STEPPER_TYPE, ALT_DRIVER_TYPE, or run the online configurator again.
#endif

#if (AZ_STEPPER_TYPE == STEPPER_TYPE_NONE)
    // 방위각 제어가 없는 기본 설정은 유효함
    #if (AZ_DRIVER_TYPE == DRIVER_TYPE_NONE)
    // 유효한 ALT 스테퍼와 드라이버 조합
    #else
        #error Defined an AZ driver, but no AZ stepper.
    #endif
#elif defined(__AVR_ATmega2560__)
    // 방위각 설정
    #if (AZ_DRIVER_TYPE == DRIVER_TYPE_TMC2209_UART)
        #ifndef AZ_DRIVER_ADDRESS
            // UART 모드의 TMC2209에는 시리얼 버스 주소가 지정되어야 함
            #error AZ driver address for DRIVER_TYPE_TMC2209_UART not specified.
        #endif
    #endif

#else
    #error Configuration does not support AZ. Use at own risk.
#endif

#if (ALT_STEPPER_TYPE == STEPPER_TYPE_NONE)
    // 고도 제어가 없는 기본 설정은 유효함
    #if (ALT_DRIVER_TYPE == DRIVER_TYPE_NONE)
    // 유효한 ALT 스테퍼와 드라이버 조합
    #else
        #error Defined an ALT driver, but no ALT stepper.
    #endif
#elif defined(__AVR_ATmega2560__)
    // 고도 설정
    #if (ALT_DRIVER_TYPE == DRIVER_TYPE_TMC2209_UART)
        #ifndef ALT_DRIVER_ADDRESS
            // UART 모드의 TMC2209에는 시리얼 버스 주소가 지정되어야 함
            #error ALT driver address for DRIVER_TYPE_TMC2209_UART not specified.
        #endif
    #endif

#else
    #warning Configuration does not support ALT. Use at own risk.
#endif

#if (FOCUS_STEPPER_TYPE == STEPPER_TYPE_NONE)
    // 초점 제어가 없는 기본 설정은 유효함
    #if (FOCUS_DRIVER_TYPE == DRIVER_TYPE_NONE)
    // 유효한 초점 스테퍼와 드라이버 조합
    #else
        #error Defined an Focus driver, but no Focus stepper.
    #endif
#elif defined(__AVR_ATmega2560__)
    // 초점 설정
    #if (FOCUS_STEPPER_TYPE == DRIVER_TYPE_TMC2209_UART)
        #ifndef FOCUS_DRIVER_ADDRESS
            // UART 모드의 TMC2209에는 시리얼 버스 주소가 지정되어야 함
            #error Focus driver address for DRIVER_TYPE_TMC2209_UART not specified.
        #endif
    #endif

#else
    #warning Configuration does not support Focus. Use at own risk.
#endif

#if (WIFI_ENABLED == 0)
// WiFi가 없는 기본 설정은 유효함
#elif defined(ESP32)
    // WiFi는 ESP32에서만 지원됨
    #if !defined(WIFI_HOSTNAME)
        #error Wifi hostname must be provided for infrastructure and AP modes
    #endif
    #if (WIFI_MODE == WIFI_MODE_DISABLED)
    // WiFi가 비활성화된 기본 설정은 유효함
    #endif
    #if (WIFI_MODE == WIFI_MODE_INFRASTRUCTURE) || (WIFI_MODE == WIFI_MODE_ATTEMPT_INFRASTRUCTURE_FAIL_TO_AP)
        #if !defined(WIFI_INFRASTRUCTURE_MODE_SSID) || !defined(WIFI_INFRASTRUCTURE_MODE_WPAKEY)
            #error Wifi SSID and WPA key must be provided for infrastructure mode
        #endif
    #elif (WIFI_MODE == WIFI_MODE_AP_ONLY) || (WIFI_MODE == WIFI_MODE_ATTEMPT_INFRASTRUCTURE_FAIL_TO_AP)
        #if !defined(WIFI_AP_MODE_WPAKEY)
            #error Wifi WPA key must be provided for AP mode
        #endif
    #else
        #error Unsupported WiFi configuration. Use at own risk.
    #endif
#else
    #error Unsupported WiFi configuration (WiFI only supported on ESP32). Use at own risk.
#endif

// 외부 센서
#if (USE_GPS == 0)
// GPS가 없는 기본 설정은 유효함
#elif defined(ESP32) || defined(__AVR_ATmega2560__)
// GPS는 ESP32와 ATmega에서 지원됨
#else
    #error Unsupported GPS configuration. Use at own risk.
#endif

#if (USE_GYRO_LEVEL == 0)
// 자이로가 없는 기본 설정은 유효함
#elif defined(ESP32) || defined(__AVR_ATmega2560__)
// 자이로는 ESP32와 ATmega에서 지원됨
#else
    #error Unsupported gyro configuration. Use at own risk.
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                            ////////
// 핀 할당 검증               ////////
//                            ////////
//////////////////////////////////////

// 모터 & 드라이버 설정
#if (DEC_DRIVER_TYPE == DRIVER_TYPE_A4988_GENERIC) || (DEC_DRIVER_TYPE == DRIVER_TYPE_TMC2209_STANDALONE)
    #if !defined(DEC_STEP_PIN) || !defined(DEC_DIR_PIN) || !defined(DEC_EN_PIN)
        // 필수 핀 할당 누락
        #error Missing pin assignments for configured DEC DRIVER_TYPE_A4988_GENERIC or DRIVER_TYPE_TMC2209_STANDALONE driver
    #endif
    #if (!defined(DEC_MS0_PIN) || !defined(DEC_MS1_PIN) || !defined(DEC_MS2_PIN))                                                          \
        && (BOARD != BOARD_AVR_MKS_GEN_L_V1 && BOARD != BOARD_AVR_RAMPS)
        #warning Missing pin assignments for MS pins
    #endif
#elif (DEC_DRIVER_TYPE == DRIVER_TYPE_TMC2209_UART)
    #if !defined(DEC_STEP_PIN) || !defined(DEC_DIR_PIN) || !defined(DEC_EN_PIN) || !defined(DEC_DIAG_PIN)
        // 필수 핀 할당 누락
        #error Missing pin assignments for configured DEC DRIVER_TYPE_TMC2209_UART driver
    #endif
    #if !((defined(DEC_SERIAL_PORT_TX) && defined(DEC_SERIAL_PORT_RX)) || defined(DEC_SERIAL_PORT))
        // UART 시리얼용 필수 핀 할당 누락
        #error Missing pin assignments for configured DEC DRIVER_TYPE_TMC2209_UART driver serial connection
    #endif
#endif

#if (RA_DRIVER_TYPE == DRIVER_TYPE_A4988_GENERIC) || (RA_DRIVER_TYPE == DRIVER_TYPE_TMC2209_STANDALONE)
    #if !defined(RA_STEP_PIN) || !defined(RA_DIR_PIN) || !defined(RA_EN_PIN)
        // 필수 핀 할당 누락
        #error Missing pin assignments for configured RA DRIVER_TYPE_A4988_GENERIC or DRIVER_TYPE_TMC2209_STANDALONE driver
    #endif
    #if (!defined(RA_MS0_PIN) || !defined(RA_MS1_PIN) || !defined(RA_MS2_PIN))                                                             \
        && (BOARD != BOARD_AVR_MKS_GEN_L_V1 && BOARD != BOARD_AVR_RAMPS)
        #warning Missing pin assignments for MS pins
    #endif
#elif (RA_DRIVER_TYPE == DRIVER_TYPE_TMC2209_UART)
    #if !defined(RA_STEP_PIN) || !defined(RA_DIR_PIN) || !defined(RA_EN_PIN) || !defined(RA_DIAG_PIN)
        // 필수 핀 할당 누락
        #error Missing pin assignments for configured RA DRIVER_TYPE_TMC2209_UART driver
    #endif
    #if !((defined(RA_SERIAL_PORT_TX) && defined(RA_SERIAL_PORT_RX)) || defined(RA_SERIAL_PORT))
        // UART 시리얼용 필수 핀 할당 누락
        #error Missing pin assignments for configured RA DRIVER_TYPE_TMC2209_UART driver serial connection
    #endif
#endif

#if (AZ_STEPPER_TYPE != STEPPER_TYPE_NONE)
    #if (AZ_DRIVER_TYPE == DRIVER_TYPE_A4988_GENERIC) || (AZ_DRIVER_TYPE == DRIVER_TYPE_TMC2209_STANDALONE)
        #if !defined(AZ_STEP_PIN) || !defined(AZ_DIR_PIN) || !defined(AZ_EN_PIN) || !defined(AZ_DIAG_PIN)
            // 필수 핀 할당 누락
            #error Missing pin assignments for configured AZ DRIVER_TYPE_A4988_GENERIC or DRIVER_TYPE_TMC2209_STANDALONE driver
        #endif
    #elif (AZ_DRIVER_TYPE == DRIVER_TYPE_TMC2209_UART)
        #if !defined(AZ_STEP_PIN) || !defined(AZ_DIR_PIN) || !defined(AZ_EN_PIN) || !defined(AZ_DIAG_PIN)
            // 필수 핀 할당 누락 (ATmega는 이 드라이버에 SoftwareSerial 사용)
            #error Missing pin assignments for configured AZ DRIVER_TYPE_TMC2209_UART driver
        #endif
        #if !((defined(AZ_SERIAL_PORT_TX) && defined(AZ_SERIAL_PORT_RX)) || defined(AZ_SERIAL_PORT))
            // UART 시리얼용 필수 핀 할당 누락
            #error Missing pin assignments for configured AZ DRIVER_TYPE_TMC2209_UART driver serial connection
        #endif
    #endif
#endif

#if (ALT_STEPPER_TYPE != STEPPER_TYPE_NONE)
    #if (ALT_DRIVER_TYPE == DRIVER_TYPE_A4988_GENERIC) || (ALT_DRIVER_TYPE == DRIVER_TYPE_TMC2209_STANDALONE)
        #if !defined(ALT_STEP_PIN) || !defined(ALT_DIR_PIN) || !defined(ALT_EN_PIN) || !defined(ALT_DIAG_PIN)
            // 필수 핀 할당 누락
            #error Missing pin assignments for configured AZ DRIVER_TYPE_A4988_GENERIC or DRIVER_TYPE_TMC2209_STANDALONE driver
        #endif
    #elif (ALT_DRIVER_TYPE == DRIVER_TYPE_TMC2209_UART)
        #if !defined(ALT_STEP_PIN) || !defined(ALT_DIR_PIN) || !defined(ALT_EN_PIN) || !defined(ALT_DIAG_PIN)
            // 필수 핀 할당 누락 (ATmega는 이 드라이버에 SoftwareSerial 사용)
            #error Missing pin assignments for configured ALT DRIVER_TYPE_TMC2209_UART driver
        #endif
        #if !((defined(ALT_SERIAL_PORT_TX) && defined(ALT_SERIAL_PORT_RX)) || defined(ALT_SERIAL_PORT))
            // UART 시리얼용 필수 핀 할당 누락
            #error Missing pin assignments for configured ALT DRIVER_TYPE_TMC2209_UART driver serial connection
        #endif
    #endif
#endif

#if (FOCUS_STEPPER_TYPE != STEPPER_TYPE_NONE)
    #if (FOCUS_DRIVER_TYPE == DRIVER_TYPE_A4988_GENERIC) || (FOCUS_DRIVER_TYPE == DRIVER_TYPE_TMC2209_STANDALONE)
        #if !defined(FOCUS_STEP_PIN) || !defined(FOCUS_DIR_PIN) || !defined(FOCUS_EN_PIN)
            // 필수 핀 할당 누락
            #error Missing pin assignments for configured Focuser DRIVER_TYPE_A4988_GENERIC or DRIVER_TYPE_TMC2209_STANDALONE driver
        #endif
    #elif (FOCUS_DRIVER_TYPE == DRIVER_TYPE_TMC2209_UART)
        #if !defined(FOCUS_STEP_PIN) || !defined(FOCUS_DIR_PIN) || !defined(FOCUS_EN_PIN)
            // 필수 핀 할당 누락 (ATmega는 이 드라이버에 SoftwareSerial 사용)
            #error Missing pin assignments for configured Focuser DRIVER_TYPE_TMC2209_UART driver
        #endif
        #if !((defined(FOCUS_SERIAL_PORT_TX) && defined(FOCUS_SERIAL_PORT_RX)) || defined(FOCUS_SERIAL_PORT))
            // UART 시리얼용 필수 핀 할당 누락
            #error Missing pin assignments for configured Focuser DRIVER_TYPE_TMC2209_UART driver serial connection
        #endif
    #endif
#endif

// 디스플레이
#if (DISPLAY_TYPE == DISPLAY_TYPE_NONE) || (DISPLAY_TYPE == DISPLAY_TYPE_LCD_KEYPAD_I2C_MCP23008)                                          \
    || (DISPLAY_TYPE == DISPLAY_TYPE_LCD_KEYPAD_I2C_MCP23017)
// I2C 외에 전용 핀이 필요하지 않음
#elif (DISPLAY_TYPE == DISPLAY_TYPE_LCD_KEYPAD)
    #if !defined(LCD_PIN4) || !defined(LCD_PIN5) || !defined(LCD_PIN6) || !defined(LCD_PIN7) || !defined(LCD_PIN8) || !defined(LCD_PIN9)
        // 필수 핀 할당 누락
        #error Missing pin assignments for configured DISPLAY_TYPE_LCD_KEYPAD display
    #endif
#elif (DISPLAY_TYPE == DISPLAY_TYPE_LCD_JOY_I2C_SSD1306)
// 디스플레이용 I2C 외에 전용 핀이 필요하지 않음
#endif

// 키패드
#if (DISPLAY_TYPE == DISPLAY_TYPE_NONE) || (DISPLAY_TYPE == DISPLAY_TYPE_LCD_KEYPAD_I2C_MCP23008)                                          \
    || (DISPLAY_TYPE == DISPLAY_TYPE_LCD_KEYPAD_I2C_MCP23017)
// I2C 외에 전용 핀이 필요하지 않음
#elif (DISPLAY_TYPE == DISPLAY_TYPE_LCD_KEYPAD)
    #if !defined(LCD_KEY_SENSE_PIN)
        // 필수 핀 할당 누락
        #error Missing sense pin assignment for configured DISPLAY_TYPE_LCD_KEYPAD keypad
    #endif
#elif (DISPLAY_TYPE == DISPLAY_TYPE_LCD_JOY_I2C_SSD1306)
    #if !defined(LCD_KEY_SENSE_X_PIN) || !defined(LCD_KEY_SENSE_Y_PIN) || !defined(LCD_KEY_SENSE_PUSH_PIN)
        // 필수 핀 할당 누락
        #error Missing sense pin assignments for configured DISPLAY_TYPE_LCD_JOY_I2C_SSD1306 joystick
    #endif
#endif

// 디버깅
#if DEBUG_SEPARATE_SERIAL == 1
    #if !defined(DEBUG_SERIAL_PORT)
        #error Missing serial port assignment for external debugging
    #endif
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                ////////
// 중요 매개변수 검증            ////////
//                                ////////
//////////////////////////////////////////

#if (DEC_DRIVER_TYPE == DRIVER_TYPE_TMC2209_UART)
    #if defined(DEC_MOTOR_CURRENT_RATING)
        #if (DEC_MOTOR_CURRENT_RATING > 2000)
            #error "The TMC2209 driver is only rated up to 2A output. Delete this error if you know what youre doing"
        #endif
        #if (DEC_MOTOR_CURRENT_RATING == 0)
            #error                                                                                                                         \
                "DEC current rating/setting cannot be zero. Please configure the current rating of your motor in you local configuration file using the DEC_MOTOR_CURRENT_RATING keyword."
        #endif
    #else
        #error                                                                                                                             \
            "DEC_MOTOR_CURRENT_RATING is not defined. Please define the current rating of your motor in you local configuration file using the DEC_MOTOR_CURRENT_RATING keyword."
    #endif
    #if defined(DEC_OPERATING_CURRENT_SETTING)
        #if (DEC_OPERATING_CURRENT_SETTING <= 0) || (DEC_OPERATING_CURRENT_SETTING > 100)
            #error "DEC_OPERATING_CURRENT_SETTING is not within acceptable range (0-100)"
        #endif
    #else
        #error                                                                                                                             \
            "DEC_OPERATING_CURRENT_SETTING is not defined. Please define the operating percentage of your motor in you local configuration file using the DEC_OPERATING_CURRENT_SETTING keyword."
    #endif
#endif

#if (RA_DRIVER_TYPE == DRIVER_TYPE_TMC2209_UART)
    #if defined(RA_MOTOR_CURRENT_RATING)
        #if (RA_MOTOR_CURRENT_RATING > 2000)
            #error "The TMC2209 driver is only rated up to 2A output. Delete this error if you know what you're doing"
        #endif
        #if (RA_MOTOR_CURRENT_RATING == 0)
            #error                                                                                                                         \
                "RA current rating/setting cannot be zero. Please configure the current rating of your motor in you local configuration file using the RA_MOTOR_CURRENT_RATING keyword."
        #endif
    #else
        #error                                                                                                                             \
            "RA_MOTOR_CURRENT_RATING is not defined. Please define the current rating of your motor in you local configuration file using the RA_MOTOR_CURRENT_RATING keyword."
    #endif
    #if defined(RA_OPERATING_CURRENT_SETTING)
        #if (RA_OPERATING_CURRENT_SETTING <= 0) || (RA_OPERATING_CURRENT_SETTING > 100)
            #error "RA_OPERATING_CURRENT_SETTING is not within acceptable range (0-100)"
        #endif
    #else
        #error                                                                                                                             \
            "RA_OPERATING_CURRENT_SETTING is not defined. Please define the operating percentage of your motor in you local configuration file using the RA_OPERATING_CURRENT_SETTING keyword."
    #endif
#endif

#if (AZ_STEPPER_TYPE != STEPPER_TYPE_NONE)
    #if !defined(AZ_MICROSTEPPING)
        #error "Azimuth microstepping must be defined. Default is 4.0f for 28BYJ steppers, 64.0f for NEMA."
    #endif
    #if (AZ_DRIVER_TYPE == DRIVER_TYPE_TMC2209_UART)
        #if defined(AZ_MOTOR_CURRENT_RATING)
            #if (AZ_MOTOR_CURRENT_RATING > 2000)
                #error "The TMC2209 driver is only rated up to 2A output. Delete this error if you know what you're doing"
            #endif
            #if (AZ_MOTOR_CURRENT_RATING == 0)
                #error                                                                                                                     \
                    "AZ current rating/setting cannot be zero. Please configure the current rating of your motor in you local configuration file using the AZ_MOTOR_CURRENT_RATING keyword."
            #endif
        #else
            #error                                                                                                                         \
                "AZ_MOTOR_CURRENT_RATING is not defined. Please define the current rating of your motor in you local configuration file using the AZ_MOTOR_CURRENT_RATING keyword."
        #endif
        #if defined(AZ_OPERATING_CURRENT_SETTING)
            #if (AZ_OPERATING_CURRENT_SETTING <= 0) || (AZ_OPERATING_CURRENT_SETTING > 100)
                #error "AZ_OPERATING_CURRENT_SETTING is not within acceptable range (0-100)"
            #endif
        #else
            #error                                                                                                                         \
                "AZ_OPERATING_CURRENT_SETTING is not defined. Please define the operating percentage of your motor in you local configuration file using the AZ_OPERATING_CURRENT_SETTING keyword."
        #endif
    #endif
#endif

#if (ALT_STEPPER_TYPE != STEPPER_TYPE_NONE)
    #if !defined(ALT_MICROSTEPPING)
        #error "Altitude microstepping must be defined. Default is 4.0f for 28BYJ steppers, 4.0f for NEMA."
    #endif
    #if (ALT_DRIVER_TYPE == DRIVER_TYPE_TMC2209_UART)
        #if defined(ALT_MOTOR_CURRENT_RATING)
            #if (ALT_MOTOR_CURRENT_RATING > 2000)
                #error "The TMC2209 driver is only rated up to 2A output. Delete this error if you know what you're doing"
            #endif
            #if (ALT_MOTOR_CURRENT_RATING == 0)
                #error                                                                                                                     \
                    "ALT current rating/setting cannot be zero. Please configure the current rating of your motor in you local configuration file using the ALT_MOTOR_CURRENT_RATING keyword."
            #endif
        #else
            #error                                                                                                                         \
                "ALT_MOTOR_CURRENT_RATING is not defined. Please define the current rating of your motor in you local configuration file using the ALT_MOTOR_CURRENT_RATING keyword."
        #endif
        #if defined(ALT_OPERATING_CURRENT_SETTING)
            #if (ALT_OPERATING_CURRENT_SETTING <= 0) || (ALT_OPERATING_CURRENT_SETTING > 100)
                #error "ALT_OPERATING_CURRENT_SETTING is not within acceptable range (0-100)"
            #endif
        #else
            #error                                                                                                                         \
                "ALT_OPERATING_CURRENT_SETTING is not defined. Please define the operating percentage of your motor in you local configuration file using the ALT_OPERATING_CURRENT_SETTING keyword."
        #endif
    #endif
#endif

#if (FOCUS_STEPPER_TYPE != STEPPER_TYPE_NONE)
    #if !defined(FOCUS_MICROSTEPPING)
        #error "Focuser microstepping must be defined. Default is 4.0f for 28BYJ steppers, 4.0f for NEMA."
    #endif
    #if (FOCUS_DRIVER_TYPE == DRIVER_TYPE_TMC2209_UART)
        #if defined(FOCUS_MOTOR_CURRENT_RATING)
            #if (FOCUS_MOTOR_CURRENT_RATING > 2000)
                #error "The TMC2209 driver is only rated up to 2A output. Delete this error if you know what you're doing"
            #endif
            #if (FOCUS_MOTOR_CURRENT_RATING == 0)
                #error                                                                                                                     \
                    "Focuser current rating/setting cannot be zero. Please configure the current rating of your motor in you local configuration file using the FOCUS_MOTOR_CURRENT_RATING keyword."
            #endif
        #else
            #error                                                                                                                         \
                "FOCUS_MOTOR_CURRENT_RATING is not defined. Please define the current rating of your motor in you local configuration file using the FOCUS_MOTOR_CURRENT_RATING keyword."
        #endif
        #if defined(FOCUS_OPERATING_CURRENT_SETTING)
            #if (FOCUS_OPERATING_CURRENT_SETTING <= 0) || (FOCUS_OPERATING_CURRENT_SETTING > 100)
                #error "FOCUS_OPERATING_CURRENT_SETTING is not within acceptable range (0-100)"
            #endif
        #else
            #error                                                                                                                         \
                "FOCUS_OPERATING_CURRENT_SETTING is not defined. Please define the operating percentage of your motor in you local configuration file using the FOCUS_OPERATING_CURRENT_SETTING keyword."
        #endif
    #endif
#endif

// OAT의 경우 자유 슬루가 작동하려면 DEC 한계가 정의되어 있어야 함
#ifndef OAM
    #ifndef DEC_LIMIT_UP
        #error "You must set DEC_LIMIT_UP to the number of degrees that your OAT can move upwards from the home position."
    #endif
    #ifndef DEC_LIMIT_DOWN
        #error "You must set DEC_LIMIT_DOWN to the number of degrees that your OAT can move downwards from the home position."
    #endif
#endif
