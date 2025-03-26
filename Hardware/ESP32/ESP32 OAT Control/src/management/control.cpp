#include "management/control.hpp"

Control& Control::getInstance() {
    static Control instance;
    return instance;
}

void Control::stopSlewing() {
    mount_serial.request("Q", false);
}

void Control::startRaDecSlewing(char dir) {
    String cmd = "M";
    cmd += dir;
    
    mount_serial.request(cmd, false);
}

void Control::stopRaDecSlewing(char dir) {
    String cmd = "Q";
    cmd += dir;
    
    mount_serial.request(cmd, false);
}

void Control::startAzAltSlewing(char dir) {
    float distance = mount_data.get_az_alt_slew_rate();
    
    String cmd = (dir == 'n' || dir == 's') ? "MAL" : "MAZ"; // {n, s} -> MAL, {e, w} -> MAZ
    cmd += (dir == 'n' || dir == 'e') ? "+" : "-"; // {n, e} -> +, {s, w} -> -

    mount_serial.request(cmd + String(distance, 1), false);
}