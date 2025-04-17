#include "gps.hpp"

GPS& GPS::getInstance() {
    static GPS instance;
    return instance;
} 