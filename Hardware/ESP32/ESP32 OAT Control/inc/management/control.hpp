#pragma once

#include "mount/serial.hpp"
#include "mount/data.hpp"

// OAT의 움직임을 모두 관리하는 곳

class Control {
private:
    static Control* instance;

    Mount::Serial& mount_serial = Mount::Serial::getInstance();
    Mount::Data& mount_data = Mount::Data::getInstance();

    // 싱글톤 패턴
    Control() {}

public:
    // Singletone
    Control(const Control&) = delete;
    Control& operator=(const Control&) = delete;

    static Control& getInstance();

    // 모든 움직임 정지
    void stopSlewing();

    // RA/DEC
    void startRaDecSlewing(char dir);
    void stopRaDecSlewing(char dir);

    // AZ/ALT
    // AZ/ALT는 눌렀을 때 한번만 이동하기 때문에 멈추는건 없음, 한번만 움직이고 자동으로 멈춤
    void startAzAltSlewing(char dir);
};
