#pragma once

/**
 * 이 파일은 고급 설정을 포함하고 있습니다. 이곳의 값을 수정하려면 충분한 지식이 있어야 합니다. 잘못된 값은 OAT의 심각한 오작동을 초래할 수 있으며, 최악의 경우 하드웨어 손상으로 이어질 수 있습니다. 여기의 기본 값은 여러 테스트 후에 선택되었으며 현재 최적의 작동을 보장합니다.
 * 
 * 
 *         이 파일을 수정할 필요가 없습니다!
 *         --------------------------------------
 **/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                            ////////
// 기타 설정                   ////////
//                            ////////
//////////////////////////////////////
#if !defined(USE_DUMMY_EEPROM)
    #define USE_DUMMY_EEPROM false
#endif
#if !defined(BUFFER_LOGS)
    #define BUFFER_LOGS false
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                            ////////
// 모터 및 드라이버 설정        ////////
//                            ////////
//////////////////////////////////////
//
// 스테퍼가 한 바퀴 회전하는 데 필요한 스텝 수입니다.
#ifndef RA_STEPPER_SPR
    #define RA_STEPPER_SPR 400  // NEMA 0.9° = 400  |  NEMA 1.8° = 200
#endif
#ifndef RA_STEPPER_SPEED
    #define RA_STEPPER_SPEED 1200  // You can change the speed and acceleration of the steppers here. Max. Speed = 3000.
#endif
#ifndef RA_STEPPER_ACCELERATION
    #define RA_STEPPER_ACCELERATION 6000
#endif

#ifndef DEC_STEPPER_SPR
    #define DEC_STEPPER_SPR 400  // NEMA 0.9° = 400  |  NEMA 1.8° = 200
#endif
#ifndef DEC_STEPPER_SPEED
    #define DEC_STEPPER_SPEED 1300  // You can change the speed and acceleration of the steppers here. Max. Speed = 3000.
#endif
#ifndef DEC_STEPPER_ACCELERATION
    #define DEC_STEPPER_ACCELERATION 6000
#endif

// 마이크로스테핑
// DRIVER_TYPE_TMC2209_UART 드라이버는 마이크로스텝 설정을 동적으로 전환할 수 있습니다.
// 다른 모든 드라이버는 MS 핀에 의해 설정되므로 여기의 값은 핀 스트래핑과 일치해야 합니다.
// 유효한 값: 1, 2, 4, 8, 16, 32, 64, 128, 256
// !! 값이 특정 드라이버의 기능과 올바른 MS 핀 구성에 해당하는지 확인하십시오
#if (RA_DRIVER_TYPE == DRIVER_TYPE_TMC2209_UART)
    #ifndef RA_SLEW_MICROSTEPPING
        #define RA_SLEW_MICROSTEPPING 8  // The (default) microstep mode used for slewing RA axis
    #endif
    #ifndef RA_TRACKING_MICROSTEPPING
        #define RA_TRACKING_MICROSTEPPING 256  // The fine microstep mode for tracking RA axis
    #endif
    #ifndef RA_UART_STEALTH_MODE
        #define RA_UART_STEALTH_MODE 0
    #endif
#elif (RA_DRIVER_TYPE == DRIVER_TYPE_A4988_GENERIC) || (RA_DRIVER_TYPE == DRIVER_TYPE_TMC2209_STANDALONE)
    #ifndef RA_SLEW_MICROSTEPPING
        #define RA_SLEW_MICROSTEPPING 8  // Microstep mode set by MS pin strapping. Use the same microstep mode for both slewing & tracking
    #endif
    #if defined(RA_TRACKING_MICROSTEPPING) && (RA_TRACKING_MICROSTEPPING != RA_SLEW_MICROSTEPPING)
        #error With A4988 drivers or TMC2209 drivers in Standalone mode, RA microstepping must be the same for slewing and tracking. Delete RA_TRACKING_MICROSTEPPING from your config.
    #endif
    #define RA_TRACKING_MICROSTEPPING RA_SLEW_MICROSTEPPING
#else
    #error Unknown RA driver type
#endif

#if (DEC_DRIVER_TYPE == DRIVER_TYPE_TMC2209_UART)
    #ifndef DEC_SLEW_MICROSTEPPING
        #define DEC_SLEW_MICROSTEPPING 16  // The (default) microstep mode used for slewing DEC
    #endif
    #ifndef DEC_GUIDE_MICROSTEPPING
        #define DEC_GUIDE_MICROSTEPPING 256  // The fine microstep mode used for guiding DEC only
    #endif
    #ifndef DEC_UART_STEALTH_MODE
        #define DEC_UART_STEALTH_MODE 0
    #endif
#elif (DEC_DRIVER_TYPE == DRIVER_TYPE_A4988_GENERIC) || (DEC_DRIVER_TYPE == DRIVER_TYPE_TMC2209_STANDALONE)
    #ifndef DEC_SLEW_MICROSTEPPING
        #define DEC_SLEW_MICROSTEPPING                                                                                                     \
            16  // Only UART drivers support dynamic switching. Use the same microstep mode for both slewing & guiding
    #endif
    #if defined(DEC_GUIDE_MICROSTEPPING) && (DEC_GUIDE_MICROSTEPPING != DEC_SLEW_MICROSTEPPING)
        #error With A4988 drivers or TMC2209 drivers in Standalone mode, DEC microstepping must be the same for slewing and guiding. Delete DEC_GUIDE_MICROSTEPPING from your config.
    #endif
    #define DEC_GUIDE_MICROSTEPPING DEC_SLEW_MICROSTEPPING
#else
    #error Unknown DEC driver type
#endif

// Extended TMC2209 UART settings
// These settings work only with TMC2209 in UART connection (single wire to TX)
#if (RA_DRIVER_TYPE == DRIVER_TYPE_TMC2209_UART) || (DEC_DRIVER_TYPE == DRIVER_TYPE_TMC2209_UART)
    #if defined(RA_RMSCURRENT) || defined(DEC_RMSCURRENT)
        #error Please delete any definitions of RA_RMSCURRENT or DEC_RMSCURRENT in your local configuration file. Define XXX_MOTOR_CURRENT_RATING and XXX_OPERATING_CURRENT_SETTING instead.
    #endif
    //UART Current settings
    #define RA_RMSCURRENT  RA_MOTOR_CURRENT_RATING *(RA_OPERATING_CURRENT_SETTING / 100.0f) / 1.414f
    #define DEC_RMSCURRENT DEC_MOTOR_CURRENT_RATING *(DEC_OPERATING_CURRENT_SETTING / 100.0f) / 1.414f

    #define RA_STALL_VALUE  100  // adjust this value if the RA autohoming sequence often false triggers, or triggers too late
    #define DEC_STALL_VALUE 10   // adjust this value if the RA autohoming sequence often false triggers, or triggers too late

    #ifndef RA_AUDIO_FEEDBACK
        #define RA_AUDIO_FEEDBACK                                                                                                          \
            0  // If one of these are set to 1, the respective driver will shut off the stealthchop mode, resulting in a audible whine
    #endif
    #ifndef DEC_AUDIO_FEEDBACK
        #define DEC_AUDIO_FEEDBACK 0  // of the stepper coils. Use this to verify that UART is working properly.
    #endif

    #ifndef USE_VREF
        #define USE_VREF 0  //By default Vref is ignored when using UART to specify rms current. Only enable if you know what you are doing.
    #endif

    #ifndef UART_CONNECTION_TEST_TXRX
        #define UART_CONNECTION_TEST_TXRX 0
    #endif
    #ifndef UART_CONNECTION_TEST_TX
        #define UART_CONNECTION_TEST_TX 0
    #endif
    #ifndef UART_CONNECTION_TEST_TX_DEG
        #define UART_CONNECTION_TEST_TX_DEG 5.0f  //Default degrees to rotate during testing
    #endif
#endif

// Backlash Settings
#ifndef BACKLASH_STEPS
    #define BACKLASH_STEPS                                                                                                                 \
        0 * RA_SLEW_MICROSTEPPING  // set the number of backlash steps the motor has (0 for NEMA motors, 16 for 28BYJ motors)
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                        ////////
// 기계 설정              ////////
//                        ////////
//////////////////////////////////

// the pitch of the GT2 timing belt
#define GT2_BELT_PITCH 2.0f  // mm

// the Circumference of the RA wheel.  V1 = 1057.1  |  V2 = 1131.0
#ifndef RA_WHEEL_CIRCUMFERENCE
    #if RA_WHEEL_VERSION == 1
        #define RA_WHEEL_CIRCUMFERENCE 1057.1f
    #elif RA_WHEEL_VERSION >= 2
        #define RA_WHEEL_CIRCUMFERENCE 1132.73f
    #else
        #error Unsupported RA wheel version, please recheck RA_WHEEL_VERSION
    #endif
#endif

// the Circumference of the DEC wheel.
#ifndef DEC_WHEEL_CIRCUMFERENCE
    #define DEC_WHEEL_CIRCUMFERENCE 565.5f
#endif

#ifndef RA_TRANSMISSION
    #define RA_TRANSMISSION (RA_WHEEL_CIRCUMFERENCE / (RA_PULLEY_TEETH * GT2_BELT_PITCH))
#endif

#ifndef RA_SLEWING_SPEED_DEG
    #define RA_SLEWING_SPEED_DEG 4.0f  // deg/s
#endif

#ifndef RA_SLEWING_ACCELERATION_DEG
    #define RA_SLEWING_ACCELERATION_DEG 4.0f  // deg/s/s
#endif

#ifndef DEC_SLEWING_SPEED_DEG
    #define DEC_SLEWING_SPEED_DEG 4.0f  // deg/s
#endif

#ifndef DEC_SLEWING_ACCELERATION_DEG
    #define DEC_SLEWING_ACCELERATION_DEG 4.0f  // deg/s/s
#endif

// RA 움직임:
// 벨트가 움직이는 표면의 반지름(V1 링에서)은 168.24mm였습니다.
// 벨트는 스테퍼 모터가 한 바퀴 회전할 때 40mm 이동합니다(2mm 피치, 20 이빨).
// RA 휠은 2 x PI x 168.24mm (V2:180mm) 둘레 = 1057.1mm (V2:1131mm)
// 한 번의 RA 회전에는 26.43 (1057.1mm / 40mm) 스테퍼 회전이 필요합니다 (V2: 28.27 (1131mm/40mm))
// 이는 108245 스텝 (26.43 x 4096)이 360도를 이동한다는 것을 의미합니다 (V2: 115812 스텝 (28.27 x 4096))
// 따라서 각도당 300.1 스텝이 있습니다 (108245 / 360)  (V2: 322 (115812 / 360))
// 이론적으로 올바른 RA 추적 속도는 1.246586 (300 x 14.95903 / 3600) (V2 : 1.333800 (322 x 14.95903 / 3600) 스텝/초 (20T 기준)
// 여기서 마이크로스테핑 비율을 포함하여 스텝/초가 스테퍼 드라이버에 대한 업데이트/초가 되도록 합니다
#ifndef RA_STEPS_PER_DEGREE
    #define RA_STEPS_PER_DEGREE (RA_TRANSMISSION * RA_STEPPER_SPR * RA_SLEW_MICROSTEPPING / 360.0f)
#endif

// RA 제한
#ifndef RA_LIMIT_LEFT
    #define RA_LIMIT_LEFT 5.0f
#endif
#ifndef RA_LIMIT_RIGHT
    #define RA_LIMIT_RIGHT 7.0f
#endif
#ifndef RA_TRACKING_LIMIT
    #define RA_TRACKING_LIMIT 7.0f
#endif

#ifndef RA_PHYSICAL_LIMIT
    #define RA_PHYSICAL_LIMIT 7.0f
#endif

#ifndef DEC_TRANSMISSION
    #define DEC_TRANSMISSION (DEC_WHEEL_CIRCUMFERENCE / (DEC_PULLEY_TEETH * GT2_BELT_PITCH))
#endif

// DEC 움직임:
// 벨트는 스테퍼 모터가 한 바퀴 회전할 때 40mm 이동합니다(2mm 피치, 20 이빨).
// DEC 휠은 2 x PI x 90mm 둘레로 565.5mm입니다
// 한 번의 DEC 회전에는 14.13 (565.5mm/40mm) 스테퍼 회전이 필요합니다
// 이는 57907 스텝 (14.14 x 4096)이 360도를 이동한다는 것을 의미합니다
// 따라서 각도당 160.85 스텝이 있습니다 (57907/360) (20T 기준)
// 여기서 마이크로스테핑 비율을 포함하여 스텝/초가 스테퍼 드라이버에 대한 업데이트/초가 되도록 합니다
#ifndef DEC_STEPS_PER_DEGREE
    #define DEC_STEPS_PER_DEGREE (DEC_TRANSMISSION * DEC_STEPPER_SPR * DEC_SLEW_MICROSTEPPING / 360.0f)
#endif

#ifndef DEC_LIMIT_UP
    #ifdef OAM
        #define DEC_LIMIT_UP 135.0f
    #else
        #define DEC_LIMIT_UP 0.0f
    #endif
#endif
#ifndef DEC_LIMIT_DOWN
    #ifdef OAM
        #define DEC_LIMIT_DOWN 135.0f
    #else
        #define DEC_LIMIT_DOWN 0.0f
    #endif
#endif

////////////////////////////
//
// 가이드 설정
// 이는 가이드 펄스가 가지는 일반 추적 속도의 배수입니다.
// 북쪽 및 남쪽(DEC) 추적 속도는 +배수 및 -배수로 계산됩니다
// 서쪽 및 동쪽(RA) 추적 속도는 (배수+1.0) 및 (배수-1.0)로 계산됩니다
#ifndef RA_PULSE_MULTIPLIER
    #define RA_PULSE_MULTIPLIER 1.5f
#endif
#ifndef DEC_PULSE_MULTIPLIER
    #define DEC_PULSE_MULTIPLIER 0.5f
#endif

////////////////////////////
//
// 축 반전
// 모터 방향을 반전하려면 1 또는 0으로 설정하세요
#ifndef RA_INVERT_DIR
    #define RA_INVERT_DIR 0
#endif
#ifndef DEC_INVERT_DIR
    #define DEC_INVERT_DIR 0
#endif
#ifndef AZ_INVERT_DIR
    #define AZ_INVERT_DIR 0
#endif
#ifndef ALT_INVERT_DIR
    #define ALT_INVERT_DIR 0
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                  ////////
// LCD 설정         ////////
//                  ////////
////////////////////////////
//
// 업데이트 시간
// 슬루잉 작업 중 LCD 화면 업데이트 간격(ms)
#define DISPLAY_UPDATE_TIME 200

////////////////////////////
//
// LCD 버튼 테스트
// 키 진단을 실행하려면 1로 설정하세요. 모든 추적기 기능이 비활성화됩니다.
#ifndef LCD_BUTTON_TEST
    #define LCD_BUTTON_TEST 0
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                     ///
// 하드웨어 확장 지원 섹션              ///
//                                     ///
//////////////////////////////////////////
//
// 하위 호환성. V1.9.07부터 방위각/고도 애드온이 각각의 개별 제어로 변경되었습니다.
//

//////////////////////////////////////////
// AutoPA 방위각 지원
//////////////////////////////////////////
#ifdef AZIMUTH_ALTITUDE_MOTORS
    #if AZIMUTH_ALTITUDE_MOTORS == 1
        #error Please remove AZIMUTH_ALTITUDE_MOTORS definition and use only ALT_STEPPER_TYPE and AZ_STEPPER_TYPE
    #endif
    #undef AZIMUTH_ALTITUDE_MOTORS
#endif

//////////////////////////////////////////
// AutoPA 방위각 지원
//////////////////////////////////////////
// 방위각 모터 기능을 로컬 설정에서 활성화하세요. 여기서는 수정하지 마세요!
#if AZ_STEPPER_TYPE != STEPPER_TYPE_NONE
    #ifndef AZ_MICROSTEPPING
        #define AZ_MICROSTEPPING 64
    #endif
    #ifndef AZ_STEPPER_SPR
        #define AZ_STEPPER_SPR 400  // NEMA 0.9° = 400  |  NEMA 1.8° = 200
    #endif
    #ifndef AZ_STEPPER_SPEED
        #define AZ_STEPPER_SPEED                                                                                                           \
            (100 * AZ_MICROSTEPPING)  // You can change the speed and acceleration of the steppers here. Max. Speed = 3000.
    #endif
    #ifndef AZ_STEPPER_ACCELERATION
        #define AZ_STEPPER_ACCELERATION (100 * AZ_MICROSTEPPING)
    #endif

    // the Circumference of the AZ rotation. 808mm dia.
    #ifndef AZ_CIRCUMFERENCE
        #define AZ_CIRCUMFERENCE 2538.4f
    #endif
    #ifndef AZIMUTH_STEPS_PER_REV
        #define AZIMUTH_STEPS_PER_REV                                                                                                      \
            (AZ_CORRECTION_FACTOR * (AZ_CIRCUMFERENCE / (AZ_PULLEY_TEETH * GT2_BELT_PITCH)) * AZ_STEPPER_SPR                               \
             * AZ_MICROSTEPPING)  // Actually u-steps/rev
    #endif
    #define AZIMUTH_STEPS_PER_ARC_MINUTE (AZIMUTH_STEPS_PER_REV / (360 * 60.0f))  // Used to determine move distance in steps

    // AZ TMC2209 UART settings
    // These settings work only with TMC2209 in UART connection (single wire to TX)
    #if (AZ_DRIVER_TYPE == DRIVER_TYPE_TMC2209_UART)
        #define AZ_RMSCURRENT AZ_MOTOR_CURRENT_RATING *(AZ_OPERATING_CURRENT_SETTING / 100.0f) / 1.414f

        #ifndef AZ_MOTOR_HOLD_SETTING
            #define AZ_MOTOR_HOLD_SETTING 100
        #endif

        #define AZ_STALL_VALUE 10  // adjust this value if the RA autohoming sequence often false triggers, or triggers too late

        #ifndef USE_VREF
            #define USE_VREF                                                                                                               \
                0  //By default Vref is ignored when using UART to specify rms current. Only enable if you know what you are doing.
        #endif
    #endif
    #ifndef AZ_ALWAYS_ON
        #define AZ_ALWAYS_ON 0
    #endif
#endif

//////////////////////////////////////////
// AutoPA 고도 지원
//////////////////////////////////////////
// 로컬 구성에서 고도 모터 기능을 활성화하세요. 여기서는 수정하지 마세요!
#if (ALT_STEPPER_TYPE != STEPPER_TYPE_NONE)
    #ifndef ALT_MICROSTEPPING
        #define ALT_MICROSTEPPING 4
    #endif
    #ifndef ALT_STEPPER_SPR
        #define ALT_STEPPER_SPR 400  // NEMA 0.9° = 400  |  NEMA 1.8° = 200
    #endif
    #ifndef ALT_STEPPER_SPEED
        #define ALT_STEPPER_SPEED 2000
    #endif
    #ifndef ALT_STEPPER_ACCELERATION
        #define ALT_STEPPER_ACCELERATION 2000
    #endif

    #ifndef AUTOPA_VERSION
        #define AUTOPA_VERSION 1
    #endif

    #ifdef OAM
        #ifndef ALT_ROD_PITCH
            #define ALT_ROD_PITCH 1.0  // mm/rev
        #endif
        // the Circumference of the AZ rotation. 209.1mm radius.
        #define ALT_CIRCUMFERENCE 209.1 * 2 * PI
        #define ALTITUDE_STEPS_PER_REV                                                                                                     \
            (ALT_CORRECTION_FACTOR * (ALT_CIRCUMFERENCE / ALT_ROD_PITCH) * ALT_STEPPER_SPR * ALT_MICROSTEPPING)  // Actually u-steps/rev

    #else
        // the Circumference of the AZ rotation. 770mm dia.
        #define ALT_CIRCUMFERENCE 2419.0f
        #if AUTOPA_VERSION == 1
            // the ratio of the ALT gearbox for AutoPA V1 (40:3)
            #define ALT_WORMGEAR_RATIO (40.0f / 3.0f)
        #else
            // the ratio of the ALT gearbox for AutoPA V2 (40:1)
            #define ALT_WORMGEAR_RATIO (40.0f)
        #endif
        #ifndef ALTITUDE_STEPS_PER_REV
            #define ALTITUDE_STEPS_PER_REV                                                                                                 \
                (ALT_CORRECTION_FACTOR * (ALT_CIRCUMFERENCE / (ALT_PULLEY_TEETH * GT2_BELT_PITCH)) * ALT_STEPPER_SPR * ALT_MICROSTEPPING   \
                 * ALT_WORMGEAR_RATIO)  // Actually u-steps/rev
        #endif
    #endif

    #ifndef ALTITUDE_STEPS_PER_ARC_MINUTE
        #define ALTITUDE_STEPS_PER_ARC_MINUTE (ALTITUDE_STEPS_PER_REV / (360 * 60.0f))  // Used to determine move distance in steps
    #endif

    // ALT TMC2209 UART settings
    // These settings work only with TMC2209 in UART connection (single wire to TX)
    #if (ALT_DRIVER_TYPE == DRIVER_TYPE_TMC2209_UART)
        #define ALT_RMSCURRENT ALT_MOTOR_CURRENT_RATING *(ALT_OPERATING_CURRENT_SETTING / 100.0f) / 1.414f

        #ifndef ALT_MOTOR_HOLD_SETTING
            #define ALT_MOTOR_HOLD_SETTING 100
        #endif

        #define ALT_STALL_VALUE 10  // adjust this value if the RA autohoming sequence often false triggers, or triggers too late

        #ifndef USE_VREF
            #define USE_VREF                                                                                                               \
                0  //By default Vref is ignored when using UART to specify rms current. Only enable if you know what you are doing.
        #endif
    #endif
    #ifndef ALT_ALWAYS_ON
        #define ALT_ALWAYS_ON 0
    #endif
#endif

//////////////////////////////////////////
// 포커서 지원
//////////////////////////////////////////
// 로컬 설정에서 포커서 기능을 활성화하세요. 여기서는 수정하지 마세요!
#if (FOCUS_STEPPER_TYPE != STEPPER_TYPE_NONE)
    #ifndef FOCUS_UART_STEALTH_MODE
        #define FOCUS_UART_STEALTH_MODE 1
    #endif
    #if FOCUS_DRIVER_TYPE != DRIVER_TYPE_NONE
        #ifndef FOCUS_MICROSTEPPING
            #define FOCUS_MICROSTEPPING 8
        #endif
    #else
        #error Unknown Focus driver type. Did you define FOCUS_DRIVER_TYPE?
    #endif
    #ifndef FOCUS_STEPPER_SPR
        #define FOCUS_STEPPER_SPR 400  // NEMA 0.9° = 400  |  NEMA 1.8° = 200
    #endif
    #ifndef FOCUS_STEPPER_SPEED
        #define FOCUS_STEPPER_SPEED 1000  // You can change the speed and acceleration of the steppers here. Max. Speed = 3000.
    #endif
    #ifndef FOCUS_STEPPER_ACCELERATION
        #define FOCUS_STEPPER_ACCELERATION 1000
    #endif

    // FOCUS TMC2209 UART settings
    // These settings work only with TMC2209 in UART connection (single wire to TX)
    #ifndef FOCUSER_ALWAYS_ON
        #define FOCUSER_ALWAYS_ON 0
    #endif

    #if (FOCUS_DRIVER_TYPE == DRIVER_TYPE_TMC2209_UART)
        #define FOCUS_RMSCURRENT FOCUS_MOTOR_CURRENT_RATING *(FOCUS_OPERATING_CURRENT_SETTING / 100.0f) / 1.414f
        #ifndef FOCUSER_MOTOR_HOLD_SETTING
            #define FOCUSER_MOTOR_HOLD_SETTING 100
        #endif
        #define FOCUS_STALL_VALUE 1  // adjust this value if the Focus autohoming sequence often false triggers, or triggers too late
        #ifndef USE_VREF
            #define USE_VREF                                                                                                               \
                0  //By default Vref is ignored when using UART to specify rms current. Only enable if you know what you are doing.
        #endif
    #endif
#endif

//////////////////////////////////////////
// RA 자동 홈 지원
//////////////////////////////////////////
// 로컬 설정에서 홈을 활성화하세요. 여기서는 수정하지 마세요!
#ifndef USE_HALL_SENSOR_RA_AUTOHOME
    #define USE_HALL_SENSOR_RA_AUTOHOME 0
#elif USE_HALL_SENSOR_RA_AUTOHOME == 1
    #ifndef RA_HOMING_SENSOR_ACTIVE_STATE
        #define RA_HOMING_SENSOR_ACTIVE_STATE LOW
    #endif
    #ifndef RA_HOMING_SENSOR_SEARCH_DEGREES
        #define RA_HOMING_SENSOR_SEARCH_DEGREES 30
    #endif
#endif

//////////////////////////////////////////
// DEC 자동 홈 지원
//////////////////////////////////////////
// 로컬 설정에서 홈을 활성화하세요. 여기서는 수정하지 마세요!
#ifndef USE_HALL_SENSOR_DEC_AUTOHOME
    #define USE_HALL_SENSOR_DEC_AUTOHOME 0
#elif USE_HALL_SENSOR_DEC_AUTOHOME == 1
    #ifndef DEC_HOMING_SENSOR_ACTIVE_STATE
        #define DEC_HOMING_SENSOR_ACTIVE_STATE LOW
    #endif
    #ifndef DEC_HOMING_SENSOR_SEARCH_DEGREES
        #define DEC_HOMING_SENSOR_SEARCH_DEGREES 30
    #endif
#endif

// RA 엔드스위치 지원
//////////////////////////////////////////
// 로컬 설정에서 RA 엔드스위치를 활성화하세요. 여기서는 수정하지 마세요!
#ifndef USE_RA_END_SWITCH
    #define USE_RA_END_SWITCH 0
#else
    #ifndef RA_END_SWITCH_ACTIVE_STATE
        #define RA_END_SWITCH_ACTIVE_STATE LOW
    #endif
    // 엔드스위치가 트리거된 후 몇 도를 되돌릴지 정의할 수 있습니다.
    // 기계적 엔드스위치는 히스테리시스 동작을 가질 수 있으며, 한 번 신호가 발생하면
    // 신호 지점 너머로 충분히 이동해야 신호가 해제될 수 있습니다.
    #ifndef RA_ENDSWITCH_BACKSLEW_DEG
        #define RA_ENDSWITCH_BACKSLEW_DEG 0.5
    #endif
#endif

//////////////////////////////////////////
// DEC 엔드스위치 지원
//////////////////////////////////////////
// 로컬 설정에서 DEC 엔드스위치를 활성화하세요. 여기서는 수정하지 마세요!
#ifndef USE_DEC_END_SWITCH
    #define USE_DEC_END_SWITCH 0
#else
    #ifndef DEC_END_SWITCH_ACTIVE_STATE
        #define DEC_END_SWITCH_ACTIVE_STATE LOW
    #endif
    // You can define how many degrees to slew back after the end switch has triggered.
    // Mechanical end switches might have a hysteresis behavior, meaning once signaled,
    // it needs to move well back beyond the signal point to become un-signaled.
    #ifndef DEC_ENDSWITCH_BACKSLEW_DEG
        #define DEC_ENDSWITCH_BACKSLEW_DEG 0.5
    #endif

#endif

//////////////////////////////////////////
// LCD 디스플레이 지원
//////////////////////////////////////////
// 로컬 설정에서 LCD 기능을 활성화하세요. 여기서는 수정하지 마세요!
#if DISPLAY_TYPE != DISPLAY_TYPE_NONE

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                         ///
// 기능 지원 섹션           ///
//     LCD 디스플레이가     ///
//       있는 마운트용      ///
//                         ///
//////////////////////////////
//
// OAT를 수동으로 시작 시 설정하는 것이 편하다면, SUPPORT_GUIDED_STARTUP을 0으로 설정하세요
// (어느 정도 사용 후에는 무엇을 해야 할지 알게 될 것입니다).
//
// POI 메뉴는 약간의 데이터 메모리를 차지할 수 있으며 필요하지 않을 수도 있습니다. 그렇다면
// SUPPORT_POINTS_OF_INTEREST를 0으로 설정할 수 있습니다.
//
//
// 전체 GO 메뉴를 지원하려면 이 값을 1로 설정하세요.
// 이 값이 0으로 설정되어 있어도 Home과 Park가 있는 GO 메뉴는 여전히 사용할 수 있습니다.
    #ifndef SUPPORT_POINTS_OF_INTEREST
        #define SUPPORT_POINTS_OF_INTEREST 1
    #endif

    // Set this to 1 to support Guided Startup
    #ifndef SUPPORT_GUIDED_STARTUP
        #define SUPPORT_GUIDED_STARTUP 1
    #endif

// Set this to 1 to support CTRL menu, allowing you to manually slew the mount with the buttons.
    #define SUPPORT_MANUAL_CONTROL 1

// Set this to 1 to support CAL menu, allowing you to calibrate various things
    #define SUPPORT_CALIBRATION 1

// Set this to 1 to support INFO menu that displays various pieces of information about the mount.
    #define SUPPORT_INFO_DISPLAY 1

#else  // No Display section

    #define SUPPORT_POINTS_OF_INTEREST 0
    #if SUPPORT_GUIDED_STARTUP == 1
        #error "Guided startup is only available with a display."
    #endif
    #define SUPPORT_GUIDED_STARTUP 0
    #define SUPPORT_MANUAL_CONTROL 0
    #define SUPPORT_CALIBRATION    0
    #define SUPPORT_INFO_DISPLAY   0
    #if SUPPORT_DRIFT_ALIGNMENT == 1
        #error "Drift Alignment is only available with a display."
    #endif
    #define SUPPORT_DRIFT_ALIGNMENT 0
#endif  // DISPLAY_TYPE

// Enable Meade protocol communication over serial
#if !defined(SUPPORT_SERIAL_CONTROL)
    #define SUPPORT_SERIAL_CONTROL 1
#endif

// The port number to access OAT control over WiFi (ESP32 only)
#define WIFI_PORT 4030

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                  ////////
// 기타 하드웨어 구성               ////////
//                                  ////////
////////////////////////////////////////////

// Stepper drivers
#if (RA_DRIVER_TYPE == DRIVER_TYPE_TMC2209_UART)
    #if defined(ESP32)
        #define RA_SERIAL_PORT Serial2  // Can be shared with DEC_SERIAL_PORT
    #elif defined(__AVR_ATmega2560__)
    // Uses SoftwareSerial
    #endif
#endif

#if (DEC_DRIVER_TYPE == DRIVER_TYPE_TMC2209_UART)
    #if defined(ESP32)
        #define DEC_SERIAL_PORT Serial2  // Can be shared with RA_SERIAL_PORT
    #elif defined(__AVR_ATmega2560__)
    // Uses SoftwareSerial
    #endif
#endif

// Focuser
#if (FOCUS_DRIVER_TYPE == DRIVER_TYPE_TMC2209_UART)
    #if defined(ESP32)
        #define FOCUS_SERIAL_PORT Serial2  // Can be shared with RA_SERIAL_PORT
    #elif defined(__AVR_ATmega2560__)
    // Uses SoftwareSerial
    #endif
#endif

// GPS
#if USE_GPS == 1
    #define GPS_BAUD_RATE 9600
#endif

////////////////////////////
//
// 디버그 출력
//
// #define DEBUG_LEVEL (DEBUG_STEPPERS|DEBUG_MOUNT)
// #define DEBUG_LEVEL (DEBUG_INFO|DEBUG_MOUNT|DEBUG_GENERAL)
// #define DEBUG_LEVEL (DEBUG_SERIAL|DEBUG_WIFI|DEBUG_INFO|DEBUG_MOUNT|DEBUG_GENERAL)
// #define DEBUG_LEVEL (DEBUG_ANY)
// #define DEBUG_LEVEL (DEBUG_INFO|DEBUG_MOUNT|DEBUG_GENERAL)
//
// 비트 이름                출력
//  0  DEBUG_INFO           일반 출력, 시작 변수 및 상태 등
//  1  DEBUG_SERIAL         시리얼 명령 및 응답
//  2  DEBUG_WIFI           와이파이 관련 출력
//  3  DEBUG_MOUNT          마운트 처리 출력
//  4  DEBUG_MOUNT_VERBOSE  자세한 마운트 처리 (좌표 등)
//  5  DEBUG_GENERAL        기타 잡다한 출력
//  6  DEBUG_MEADE          Meade 명령 처리 출력
// OAT가 시리얼 포트로 보내야 하는 디버그 출력의 양을 지정하려면 이 값을 설정하세요.
// OAT를 제어하는 앱을 사용하는 경우, 어떤 디버그 출력도 해당 앱을 혼란스럽게 할 수 있습니다.
// 디버그 출력은 OAT를 제어하기 위해 와이파이를 사용하거나 터미널을 통해 수동 명령을 발행할 때 유용합니다.
#ifndef DEBUG_LEVEL
    #define DEBUG_LEVEL (DEBUG_NONE)
#endif

#ifndef DEBUG_SEPARATE_SERIAL
    #define DEBUG_SEPARATE_SERIAL 0
#endif

#ifndef DEBUG_SERIAL_BAUDRATE
    #define DEBUG_SERIAL_BAUDRATE 115200
#endif

#if defined(OAT_DEBUG_BUILD)
    // AVR based boards have numbers < 1000
    #if BOARD < 1000
        /*
     * Debugging on the mega2560 using avr-stub dissallows application-code from
     * using the normal (USB) serial port
     */
        // Disable debug output
        #if defined(DEBUG_LEVEL)
            #undef DEBUG_LEVEL
        #endif
        #define DEBUG_LEVEL (DEBUG_NONE)

        // Disable serial control
        #if defined(SUPPORT_SERIAL_CONTROL)
            #undef SUPPORT_SERIAL_CONTROL
        #endif
        #define SUPPORT_SERIAL_CONTROL 0
    #else
        #error "Debugging not supported on this platform"
    #endif
#endif
