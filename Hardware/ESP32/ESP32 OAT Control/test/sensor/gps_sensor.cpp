#include <SoftwareSerial.h>
#include <string>

class GPSSensor {
public:
    GPSSensor(int rx, int tx);

    bool read(uint8_t &data);

private:
    SoftwareSerial ss;
};

GPSSensor::GPSSensor(int rx, int tx) : ss(rx, tx) { // ss 초기화는 콜론 저걸로 초기화 방법 밖에 없나봄
    ss.begin(9600);
}

bool GPSSensor::read(uint8_t &data) {
    if(ss.available()) {
        data = ss.read();
        return true;
    }
    return false;
}