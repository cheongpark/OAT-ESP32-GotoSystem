#include "management/initialize.hpp"

void Initialize::init() {
    this->connectMount();
}

bool Initialize::connectMount() {
    Mount::Response response; 
    
    // 제품 이름 요청
    response = mount_serial.request("GVP");
    if (response.success) {
        Mount::Data::set_info_product_name(response.message);
    }

    // 펌웨어 버전 요청
    response = mount_serial.request("GVN");
    if (response.success) {
        Mount::Data::set_info_firmware_version(response.message);
    }

    // 하드웨어 상태 정보 요청
    response = mount_serial.request("XGM");
    if (response.success) {
        Mount::Data::set_info_state(response.message);
    }
    
    // 펌웨어 버전 확인 (스테퍼 모터 정보는 최신 펌웨어에서만 사용 가능)
    if (Mount::Data::get_info_firmware_version_num() > 10875) {
        // 스테퍼 모터 정보 요청
        response = mount_serial.request("XGMS");
        if (response.success) {
            Mount::Data::set_stepper_info(response.message);
        }
    }

    if (true) {
        // 모든 정보 출력
        LOG(LOG_INFO, "=== 마운트 정보 ===");
        LOG(LOG_INFO, "제품명: " + Mount::Data::get_info_product_name());
        LOG(LOG_INFO, "펌웨어 버전: " + Mount::Data::get_info_firmware_version() + " (" + String(Mount::Data::get_info_firmware_version_num()) + ")");
        LOG(LOG_INFO, "보드: " + Mount::Data::get_info_state_board());
        LOG(LOG_INFO, "RA 스테퍼: " + Mount::Data::get_info_state_ra_stepper());
        LOG(LOG_INFO, "DEC 스테퍼: " + Mount::Data::get_info_state_dec_stepper());
        LOG(LOG_INFO, "디스플레이: " + Mount::Data::get_info_state_display());
        
        // 기능 플래그
        String features = "";
        if (Mount::Data::has_gps()) features += "GPS, ";
        if (Mount::Data::has_alt() && Mount::Data::has_az()) features += "AutoPA, ";
        else {
            if (Mount::Data::has_alt()) features += "MotorALT, ";
            if (Mount::Data::has_az()) features += "MotorAZ, ";
        }
        if (Mount::Data::has_gyro()) features += "Digital Level, ";
        if (Mount::Data::has_focuser()) features += "Focuser, ";
        if (Mount::Data::has_ra_hall_sensor()) features += "RA Auto Home, ";
        if (Mount::Data::has_dec_hall_sensor()) features += "DEC Auto Home, ";
        
        // 마지막 ", " 제거
        if (features.length() > 2) {
            features = features.substring(0, features.length() - 2);
        } else {
            features = "없음";
        }
        
        LOG(LOG_INFO, "기능: " + features);
        
        // 스테퍼 정보 (펌웨어 버전이 높을 때만)
        if (Mount::Data::get_info_firmware_version_num() > 10875) {
            LOG(LOG_INFO, "=== 스테퍼 모터 정보 ===");
            LOG(LOG_INFO, "RA 드라이버: " + Mount::Data::get_stepper_ra_driver());
            LOG(LOG_INFO, "DEC 드라이버: " + Mount::Data::get_stepper_dec_driver());
            LOG(LOG_INFO, "RA Slew 마이크로스텝: " + Mount::Data::get_stepper_ra_slew_ms());
            LOG(LOG_INFO, "RA Track 마이크로스텝: " + Mount::Data::get_stepper_ra_track_ms());
            LOG(LOG_INFO, "DEC Slew 마이크로스텝: " + Mount::Data::get_stepper_dec_slew_ms());
            LOG(LOG_INFO, "DEC Guide 마이크로스텝: " + Mount::Data::get_stepper_dec_guide_ms());
        }
    }

    // 초기에 모든 움직임 정지
    control.stopSlewing();
    
    return true;
}
