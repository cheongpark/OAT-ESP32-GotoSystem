/**
* 설정은 3개의 파일에 걸쳐 순서대로 누적됩니다: Configuration_local.hpp, Configuration.hpp, 그리고 Configuration_adv.hpp.
*  - Configuration_local.hpp (아래 참조)는 로컬(주로 하드웨어) 설정을 캡처합니다.
*  - Configuration.hpp (이 파일)는 Configuration_local.hpp에서 누락된 데이터에 대한 합리적인 기본값을 추가합니다.
*  - Configuration_adv.hpp는 고급 설정 데이터를 보유하며, 일반적으로 변경할 필요가 없습니다.
* 
* 이 파일에는 사용자가 지정한 매개변수가 없어야 합니다 - 모든 것은 Configuration_local.hpp에 지정되어야 합니다.
*
* 우리는 로컬 설정을 지원하므로 하드웨어와 핀아웃으로 한 번 설정하면
* Git에서 새 버전이 설정을 덮어쓰는 것에 대해 걱정할 필요가 없습니다.
* 로컬 설정 파일을 정의하는 여러 방법이 있습니다:
*  - 모든 보드/하드웨어 설정의 경우:
*    Configuration_local.hpp라는 파일을 만드세요 (https://config.openastrotech.com/ 방문이 가장 좋음)
*  - 특정 보드의 경우:
*    Configuration_local_<board>.hpp라는 파일을 만드세요, LocalConfiguration.hpp에서 유효한 보드 유형을 확인하세요
*    코드는 컴파일 시 올바른 것을 자동으로 선택합니다. 이는 개발자이거나
*    여러 OAT를 가지고 있는 경우에 유용합니다.
*  - 사용자 정의 설정 또는 고급 빌드:
*    Configuration_local.hpp를 사용하여 명령줄에 지정된 전처리기 지시문에 따라 적절한 로컬 설정 파일을 포함하세요.
*    이는 여러 OAT가 있거나 동일한 플랫폼에서 여러 소프트웨어 설정으로 소프트웨어를 빌드하려는 경우에 유용합니다.
* 
* 이 파일들은 Git에 의해 추적되지 않으므로 브랜치 변경이나 코드 업데이트 후에도 유지됩니다.
*/
#pragma once

// 다양한 기능에 대한 명명된 상수 포함
#include "Constants.hpp"

// 현재 소프트웨어 버전 포함
#include "Version.h"

// 사용자별 로컬 설정 포함
#include "LocalConfiguration.hpp"

/**
 * 사용자가 제공하지 않은 매개변수에 대한 기본값을 사용합니다.
 */

// Arduino IDE에서 빌드하는 경우 정의를 주석 해제하고 보드를 설정하세요(유효한 값은 Constants.hpp 참조).
// PlatformIO는 이 값을 자동으로 설정하므로 조치가 필요하지 않습니다.
#ifndef BOARD
// #define BOARD BOARD_AVR_MEGA2560
#endif

#ifndef BOARD
    #error You have to specify the board
#endif

// 기본값으로 북반구 설정
#ifndef NORTHERN_HEMISPHERE
    #define NORTHERN_HEMISPHERE 1
#endif

/**
 * @brief 디스플레이 & 키패드 설정
 * 지원되는 DISPLAY_TYPE 옵션은 Constants.hpp를 참조하세요.
 * 핀 할당은 디스플레이 & 키패드 선택에 따라 다릅니다.
 */
#ifndef DISPLAY_TYPE
    #define DISPLAY_TYPE DISPLAY_TYPE_NONE
#endif

#ifndef INFO_DISPLAY_TYPE
    #define INFO_DISPLAY_TYPE INFO_DISPLAY_TYPE_NONE
#elif (INFO_DISPLAY_TYPE == INFO_DISPLAY_TYPE_I2C_SSD1306_128x64)
    #ifndef INFO_DISPLAY_I2C_ADDRESS
        #define INFO_DISPLAY_I2C_ADDRESS 0x3C
    #endif
    #ifndef INFO_DISPLAY_I2C_SDA_PIN
        #define INFO_DISPLAY_I2C_SDA_PIN 5
    #endif
    #ifndef INFO_DISPLAY_I2C_SCL_PIN
        #define INFO_DISPLAY_I2C_SCL_PIN 4
    #endif
#endif

// 사용된 RA 휠 버전. 2020년 3월 이전에 OAT를 인쇄하지 않았다면
// 버전 2 이상을 사용 중입니다(소프트웨어는 1과 1보다 큰 것만 구분)
#ifndef RA_WHEEL_VERSION
    #define RA_WHEEL_VERSION 4
#endif

/**
 * @brief 각 축에서 사용되는 스테퍼 모터 유형
 * 지원되는 옵션은 Constants.hpp를 참조하세요.
 */
#ifndef RA_STEPPER_TYPE
    #define RA_STEPPER_TYPE STEPPER_TYPE_ENABLED
#endif
#ifndef DEC_STEPPER_TYPE
    #define DEC_STEPPER_TYPE STEPPER_TYPE_ENABLED
#endif
#ifndef AZ_STEPPER_TYPE
    #define AZ_STEPPER_TYPE STEPPER_TYPE_NONE
#endif
#ifndef ALT_STEPPER_TYPE
    #define ALT_STEPPER_TYPE STEPPER_TYPE_NONE
#endif
#ifndef FOCUS_STEPPER_TYPE
    #define FOCUS_STEPPER_TYPE STEPPER_TYPE_NONE
#endif

/**
 * @brief 각 축에서 사용되는 스테퍼 드라이버 유형
 * 지원되는 DRIVER_TYPE 옵션은 Constants.hpp를 참조하세요.
 */
#ifndef RA_DRIVER_TYPE
    #define RA_DRIVER_TYPE DRIVER_TYPE_TMC2209_UART
#endif
#ifndef DEC_DRIVER_TYPE
    #define DEC_DRIVER_TYPE DRIVER_TYPE_TMC2209_UART
#endif
#ifndef AZ_DRIVER_TYPE
    #define AZ_DRIVER_TYPE DRIVER_TYPE_NONE
#endif
#ifndef ALT_DRIVER_TYPE
    #define ALT_DRIVER_TYPE DRIVER_TYPE_NONE
#endif
#ifndef FOCUS_DRIVER_TYPE
    #define FOCUS_DRIVER_TYPE DRIVER_TYPE_NONE
#endif

// 풀리 톱니 수. 구매한(알루미늄) 것은 16, 인쇄한 것은 20입니다.
#ifndef RA_PULLEY_TEETH
    #define RA_PULLEY_TEETH 16
#endif
#ifndef DEC_PULLEY_TEETH
    #define DEC_PULLEY_TEETH 16
#endif
#ifndef AZ_PULLEY_TEETH
    #define AZ_PULLEY_TEETH 16
#endif
#ifndef ALT_PULLEY_TEETH
    #define ALT_PULLEY_TEETH 16
#endif

// Alt/Az 각초/스텝 값을 보정하기 위해 이 인자들을 설정하세요
#ifndef AZ_CORRECTION_FACTOR
    #define AZ_CORRECTION_FACTOR 1.0000f
#endif
#ifndef ALT_CORRECTION_FACTOR
    #define ALT_CORRECTION_FACTOR 1.0000f
#endif

/**
 * @brief GPS 수신기 설정
 * GPS를 활성화하려면 USE_GPS를 1로 설정하고, GPS를 구성에서 제외하려면 0 또는 #undef로 설정하세요.
 * ATmega에서 GPS는 하드웨어 Serial1을 사용합니다. 추가 핀이 필요하지 않습니다. configuration_adv.hpp에서 변경하세요.
 * ESP32에서 GPS는 하드웨어 Serial2를 사용합니다. 추가 핀이 필요하지 않습니다. configuration_adv.hpp에서 변경하세요.
 * 스테퍼 드라이버 DRIVER_TYPE_TMC2209_UART를 사용하는 경우 시리얼 포트 할당 충돌 가능성에 주의하세요.
 */
#ifndef USE_GPS
    #define USE_GPS 0
#endif
#ifndef GPS_BAUD_RATE
    #define GPS_BAUD_RATE 9600
#endif

/**
 * @brief 외부(USB) 시리얼 포트 설정
 * 미리 정의된 SERIAL_BAUDRATE 옵션은 Constants.hpp를 참조하거나 필요에 따라 사용자 정의하세요.
 */
#ifndef SERIAL_BAUDRATE
    #if USE_ESP32_CONTROL == 1
        #define SERIAL_BAUDRATE SERIAL_BAUDRATE_ESP32 // ESP32로 통신할 때 비트레이트를 다르게 할 수도 있기 때문에
    #else
        #define SERIAL_BAUDRATE SERIAL_BAUDRATE_ASCOM
    #endif
#endif

/**
 * @brief WiFi 설정
 * WiFi는 ESP32에서만 지원됩니다.
 * WiFi를 활성화하려면 WIFI_ENABLED를 1로 설정하고, WiFi를 구성에서 제외하려면 0 또는 #undef로 설정하세요.
 * WiFi가 활성화된 경우 WIFI_MODE와 WIFI_HOSTNAME을 설정해야 합니다.
 * WIFI_MODE 요구사항:
 *  WIFI_MODE_DISABLED (WiFi 송수신기 비활성화)
 *      추가 요구사항 없음
 *  WIFI_MODE_INFRASTRUCTURE (기존 WiFi 네트워크에 OAT 연결):
 *      WIFI_INFRASTRUCTURE_MODE_SSID & WIFI_INFRASTRUCTURE_MODE_WPAKEY 설정 필요
 *  WIFI_MODE_AP_ONLY (OAT를 WiFi 핫스팟으로 설정): 
 *      WIFI_AP_MODE_WPAKEY 설정 필요
 *  WIFI_MODE_ATTEMPT_INFRASTRUCTURE_FAIL_TO_AP (WIFI_MODE_INFRASTRUCTURE 시도, WIFI_MODE_AP_ONLY로 폴백):
 *      WIFI_MODE_INFRASTRUCTURE와 WIFI_MODE_AP_ONLY 모두의 요구사항 충족 필요
 * WIFI_INFRASTRUCTURE_MODE_WPAKEY & WIFI_AP_MODE_WPAKEY는 8자 이상 32자 이하여야 하며
 * 특수 문자나 공백을 사용하지 마세요 (ESP32 제한).
 * WiFi 활성화는 플래시 사용량을 약 420kB 증가시킵니다.
 */
#ifndef WIFI_ENABLED
    #define WIFI_ENABLED 0
#endif
#ifndef WIFI_MODE
    #define WIFI_MODE WIFI_MODE_DISABLED
#endif
#if !defined(WIFI_HOSTNAME)
    #define WIFI_HOSTNAME "OAT"
#endif
#if !defined(WIFI_INFRASTRUCTURE_MODE_SSID)
    #define WIFI_INFRASTRUCTURE_MODE_SSID ""
#endif
#if !defined(WIFI_INFRASTRUCTURE_MODE_WPAKEY)
    #define WIFI_INFRASTRUCTURE_MODE_WPAKEY ""
#endif
#if !defined(WIFI_AP_MODE_WPAKEY)
    #define WIFI_AP_MODE_WPAKEY ""
#endif

/**
 * @brief 자이로 기반 기울기/롤 수평 조정 설정
 * 자이로를 활성화하려면 USE_GYRO_LEVEL을 1로 설정하고, 자이로를 구성에서 제외하려면 0 또는 #undef로 설정하세요.
 * ATmega & ESP32에서 자이로는 하드웨어 I2C를 사용합니다. 추가 핀이 필요하지 않습니다.
 */
#ifndef USE_GYRO_LEVEL
    #define USE_GYRO_LEVEL 0
#endif

// 자이로가 롤과 피치가 잘못된 방향으로 장착된 경우 이를 1로 설정하세요
#ifndef GYRO_AXIS_SWAP
    #define GYRO_AXIS_SWAP 1
#endif

// 이슬 방지 히터 출력 활성화 (MOSFET이 있는 보드용)
#ifndef DEW_HEATER
    #define DEW_HEATER 0
#endif

#ifndef SIDEREAL_SECONDS_PER_DAY
    #define SIDEREAL_SECONDS_PER_DAY 86164.0905f
#endif

// 이 값들은 초기 정렬 중 현재 위치를 계산하는 데 필요합니다.
// Stellarium과 같은 프로그램을 사용하여 JNow(현재 날짜) 변형에서 북극성의 RA를 찾으세요.
// 이는 몇 주에 걸쳐 약간 변경되므로 몇 달마다 조정하세요.
// 이 값은 2022년 2월 7일 기준이며, 다음 조정은 2022년 말에 제안됩니다.
// DEC 좌표에도 동일한 작업을 할 수 있지만 향후 5년 동안 크게 변경되지 않습니다.
#ifndef POLARIS_RA_HOUR
    #define POLARIS_RA_HOUR 3
#endif
#ifndef POLARIS_RA_MINUTE
    #define POLARIS_RA_MINUTE 0
#endif
#ifndef POLARIS_RA_SECOND
    #define POLARIS_RA_SECOND 8
#endif

// 부팅 시 기본적으로 추적 켜기
#ifndef TRACK_ON_BOOT
    #define TRACK_ON_BOOT 1
#endif

// OAT가 시리얼 포트로 보내야 하는 디버그 출력량을 지정하세요.
// 앱을 사용하여 OAT를 제어하는 경우, 디버그 출력이 해당 앱을 혼란스럽게 할 수 있습니다.
// 디버그 출력은 WiFi로 OAT를 제어하거나 터미널을 통해 수동 명령을 실행할 때 유용합니다.
#ifndef DEBUG_LEVEL
    #define DEBUG_LEVEL (DEBUG_NONE)
#endif

// 보드별 핀 데이터 추가
#if (BOARD == BOARD_AVR_MEGA2560)
    #include "boards/AVR_MEGA2560/pins_MEGA2560.hpp"
#elif (BOARD == BOARD_AVR_RAMPS)
    #include "boards/RAMPS/pins_RAMPS.hpp"
#elif (BOARD == BOARD_ESP32_ESP32DEV)
    #include "boards/ESP32_ESP32DEV/pins_ESP32DEV.hpp"
#elif (BOARD == BOARD_AVR_MKS_GEN_L_V1)
    #include "boards/AVR_MKS_GEN_L_V1/pins_MKS_GEN_L_V1.h"
#elif (BOARD == BOARD_AVR_MKS_GEN_L_V2)
    #include "boards/AVR_MKS_GEN_L_V2/pins_MKS_GEN_L_V2.h"
#elif (BOARD == BOARD_AVR_MKS_GEN_L_V21)
    #include "boards/AVR_MKS_GEN_L_V21/pins_MKS_GEN_L_V21.h"
#endif

#include "Configuration_adv.hpp"
#include "ConfigurationValidation.hpp"