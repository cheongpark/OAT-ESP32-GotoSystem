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

## GY-87

| ESP32   | GY-87 |
|---------|-------|
| VCC     | 3.3V  |
| GND     | GND   |
| 22(SCL) | SCL   |
| 21(SDA) | SDA   |

## GPS Neo M8N (Serial1)

| ESP32   | GPS Neo M8N |
|---------|-------------|
| VCC     | 3.3V        |
| GND     | GND         |
| SD3(TX) | RX          |
| SD2(RX) | TX          |

## 설명

원래는 Serial1에 연결해야하는데.. Serial1은 연결할려면 뭔가 설정을 해야함..
그래서 일단 예시로 Serial2에 연결할 것