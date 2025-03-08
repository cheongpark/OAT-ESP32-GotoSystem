#pragma once

/**
 * 이 파일에는 OAT 사용자가 변경해서는 안 되는 상수가 포함되어 있습니다!
 * 개발자이고 새로운 하드웨어 지원을 추가하려는 경우, 
 * 증가된 값으로 여기에 적절한 정의를 추가하세요.
 **/

/**
 * 지원되는 보드. 이름은 플랫폼과 보드 이름(모델)으로 구성됩니다.
 **/
// AVR 기반 보드
#define BOARD_AVR_MEGA2560      0001
#define BOARD_AVR_MKS_GEN_L_V21 0002
#define BOARD_AVR_MKS_GEN_L_V2  0003
#define BOARD_AVR_MKS_GEN_L_V1  0004
#define BOARD_AVR_RAMPS         0005

// ESP32 기반 보드
#define BOARD_ESP32_ESP32DEV 1001

/**
 * 지원되는 키패드/디스플레이 유형. 사용하는 디스플레이와 키패드에 맞는 DISPLAY_TYPE 설정에 이 값들 중 하나를 사용하세요.
 * 
 * DISPLAY_TYPE_NONE:                       디스플레이 없음. 디스플레이를 사용하지 않는 경우 이것을 사용하세요.
 * DISPLAY_TYPE_LCD_KEYPAD:                 Arduino UNO / Mega 보드에 직접 장착할 수 있는 1602 LCD 키패드 쉴드.
 *                                          예시: https://www.digikey.com/en/products/detail/dfrobot/DFR0009/7597118
 * DISPLAY_TYPE_LCD_KEYPAD_I2C_MCP23008:    MCP23008 I/O 확장기 기반의 RGB LCD 키패드 쉴드. Arduino UNO / Mega 보드에 
 *                                          직접 장착할 수 있으며 I2C를 통해 제어됩니다.
 * DISPLAY_TYPE_LCD_KEYPAD_I2C_MCP23017:    MCP23017 I/O 확장기 기반의 RGB LCD 키패드 쉴드. Arduino UNO / Mega 보드에 
 *                                          직접 장착할 수 있으며 I2C를 통해 제어됩니다.                                         
 * DISPLAY_TYPE_LCD_JOY_I2C_SSD1306:        SSD1306 컨트롤러가 있는 I2C 32x128 OLED 디스플레이 모듈과 미니 조이스틱
 *                                          디스플레이: https://www.banggood.com/Geekcreit-0_91-Inch-128x32-IIC-I2C-Blue-OLED-LCD-Display-DIY-Module-SSD1306-Driver-IC-DC-3_3V-5V-p-1140506.html
 *                                          조이스틱: https://www.banggood.com/3pcs-JoyStick-Module-Shield-2_54mm-5-pin-Biaxial-Buttons-Rocker-for-PS2-Joystick-Game-Controller-Sensor-p-1586026.html
 **/
#define DISPLAY_TYPE_NONE                    0
#define DISPLAY_TYPE_LCD_KEYPAD              1
#define DISPLAY_TYPE_LCD_KEYPAD_I2C_MCP23008 2
#define DISPLAY_TYPE_LCD_KEYPAD_I2C_MCP23017 3
#define DISPLAY_TYPE_LCD_JOY_I2C_SSD1306     4

/**
 * 지원되는 정보 디스플레이 유형. 사용하는 디스플레이에 맞는 INF_DISPLAY_TYPE 설정에 이 값들 중 하나를 사용하세요.
 * 
 * INFO_DISPLAY_TYPE_NONE:                  디스플레이 없음. 디스플레이를 사용하지 않는 경우 이것을 사용하세요.
 * INFO_DISPLAY_TYPE_SSD1306_I2C_128x64:    I2C를 통해 연결되는 SSD1306 컨트롤러가 있는 I2C 128x64 OLED 디스플레이 모듈
 *                                          Amazon: https://www.amazon.com/dp/B06XRBTBTB?_encoding=UTF8&psc=1&ref_=cm_sw_r_cp_ud_dp_DQCWKZ7YB40X84RZSHJ0
 **/
#define INFO_DISPLAY_TYPE_NONE               0
#define INFO_DISPLAY_TYPE_I2C_SSD1306_128x64 1

// 지원되는 스테퍼 모터 모델
#define STEPPER_TYPE_NONE    -1
#define STEPPER_TYPE_ENABLED 1

// 지원되는 스테퍼 드라이버 모델
#define DRIVER_TYPE_NONE               -1
#define DRIVER_TYPE_A4988_GENERIC      1  // 고정 마이크로스테핑 지원
#define DRIVER_TYPE_TMC2209_STANDALONE 2  // 고정 마이크로스테핑 지원
#define DRIVER_TYPE_TMC2209_UART       3  // 동적 마이크로스테핑 지원

// 외부 컨트롤러에 따른 USB 시리얼 포트 속도
#define SERIAL_BAUDRATE_ASCOM 19200
#define SERIAL_BAUDRATE_ESP32 2400 // ESP32는 레벨 시프터가 필요하기에 임시로 가능한 비트레이트로 설정

// Wifi 작동 모드 (ESP32 전용)
#define WIFI_MODE_INFRASTRUCTURE 0  // 인프라 전용 - OAT가 기존 라우터에 연결
#define WIFI_MODE_AP_ONLY        1  // AP 모드 전용 - OAT가 로컬 라우터/핫스팟으로 작동
#define WIFI_MODE_ATTEMPT_INFRASTRUCTURE_FAIL_TO_AP                                                                                        \
    2                         // 인프라 시도, AP 모드로 실패 - 인프라 모드 시도 후 AP 모드로 전환
#define WIFI_MODE_DISABLED 3  // Wifi 비활성화, 송수신기 꺼짐

// 디버깅 출력 제어
// 디버그 레벨의 각 비트는 활성화할 디버그 종류를 지정합니다. 필요한 정보에 대해 이들을 DEBUG_LEVEL로 결합하세요. 예:
//  #define DEBUG_LEVEL (DEBUG_STEPPERS|DEBUG_MOUNT)
//  #define DEBUG_LEVEL (DEBUG_INFO|DEBUG_MOUNT|DEBUG_GENERAL)
//  #define DEBUG_LEVEL (DEBUG_SERIAL|DEBUG_WIFI|DEBUG_INFO|DEBUG_MOUNT|DEBUG_GENERAL)
//  #define DEBUG_LEVEL (DEBUG_ANY)
//  #define DEBUG_LEVEL (DEBUG_INFO|DEBUG_MOUNT|DEBUG_GENERAL)
#define DEBUG_NONE          0x0000  // 디버그 출력 없음 (릴리스 빌드)
#define DEBUG_INFO          0x0001  // 일반 출력, 시작 변수 및 상태 등
#define DEBUG_SERIAL        0x0002  // 시리얼 명령 및 응답
#define DEBUG_WIFI          0x0004  // Wifi 관련 출력
#define DEBUG_MOUNT         0x0008  // 마운트 처리 출력
#define DEBUG_MOUNT_VERBOSE 0x0010  // 상세 마운트 처리 (좌표 등)
#define DEBUG_GENERAL       0x0020  // 기타 기타 출력
#define DEBUG_MEADE         0x0040  // Meade 명령 처리 출력
#define DEBUG_VERBOSE       0x0080  // 고속 마운트 활동, 스테퍼 서비스 포함
#define DEBUG_STEPPERS      0x0100  // 스테퍼 모터 관련 활동
#define DEBUG_EEPROM        0x0200  // 비휘발성 설정 데이터 저장 및 검색
#define DEBUG_GYRO          0x0400  // 자이로 활동 (기울기/롤) 보정
#define DEBUG_GPS           0x0800  // GPS 활동
#define DEBUG_FOCUS         0x1000  // 포커서 활동
#define DEBUG_COORD_CALC    0x2000  // 좌표 계산
#define DEBUG_DISPLAY       0x4000  // 정보 디스플레이
#define DEBUG_GUIDE         0x8000  // 가이딩 정보
#define DEBUG_ANY           0xFFFF  // 모든 디버그 출력
