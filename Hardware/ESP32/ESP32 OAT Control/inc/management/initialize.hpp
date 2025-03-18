#pragma once

#include "mount/serial.hpp"
#include "mount/data.hpp"

class Initialize {
private:
    Mount::Serial& serial;
    
    bool connectMount();

public:
    Initialize();

    void init();
};
