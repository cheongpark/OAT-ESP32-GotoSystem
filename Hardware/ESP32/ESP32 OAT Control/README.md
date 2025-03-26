# ESP32 Pin Connection

## MKS Board UART (Serial2)

| ESP32  | Level Shifter Low | Level Shifter High | MKS Gen L 2.1 (AUX - 1) |
|--------|-------------------|--------------------|-------------------------|
| VCC    | LV                | HV                 | VCC                     |
| GND    | GND               | GND                | GND                     |
| 16(RX) | RXO               | RXI                | TX                      |
| 17(TX) | TXI               | TXO                | RX                      |

### 설명
ESP32와 MKS Gen L 2.1은 각각 3.3V 로직, 5V 로직으로 다르기 때문에
UART 통신의 경우 통신의 문제가 생긴다.
그렇기 때문에 Level Shifter으로 해결 가능

Level Shifter High와 MKS Gen L 2.1의 TX와 RX는 반대로 연결해야함
그래야 통신됨

## GY-87 (I2C)

| ESP32   | GY-87 |
|---------|-------|
| VCC     | 3.3V  |
| GND     | GND   |
| 22(SCL) | SCL   |
| 21(SDA) | SDA   |

## 설명

아래에서 설명한 것 처럼 이 I2C 통신의 핀 번호도 변경이 가능한가봄..
https://randomnerdtutorials.com/esp32-i2c-communication-arduino-ide/

## GPS Neo M8N (Serial1)

| ESP32   | GPS Neo M8N |
|---------|-------------|
| VCC     | 3.3V        |
| GND     | GND         |
| 23(TX) | RX          |
| 19(RX) | TX          |

## 설명

https://randomnerdtutorials.com/esp32-pinout-reference-gpios/ 여기에 설명된 대로 
대부분의 핀은 UART로 사용 가능
다만 UART는 3개만 이용 가능함, 그 중에서 1개는 핀이 고정되어있고 (아마도 USB 시리얼 통신 그거일 듯)
나머지 2개가 핀 변경이 가능함, 가능한 것은 저 웹페이지에 체크표시로 뭐가 가능한지 나와있음
그래서 변경해서 사용할 것임.. 기존에는 SD2, SD3를 사용할려 했는데 SPI 뭐시기 때문에 사용하지 않는 것이 좋다고 해서 변경

(참고로 GPS는 정밀한 현재 시간도 알려줌 UTC 기준)