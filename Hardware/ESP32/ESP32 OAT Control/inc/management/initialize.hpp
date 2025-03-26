#pragma once

#include "mount/serial.hpp"
#include "mount/data.hpp"
#include "control.hpp"

class Initialize {
private:
    Mount::Serial& mount_serial = Mount::Serial::getInstance();

    Control& control = Control::getInstance();
    
    bool connectMount();

public:
    Initialize() {}

    void init();
};
