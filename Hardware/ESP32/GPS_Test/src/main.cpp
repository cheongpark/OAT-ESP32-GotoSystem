#include <Arduino.h>
#include <Servo.h>

const int PWM_PIN = 3;
Servo servo;

void setup() {
  servo.attach(PWM_PIN);
  delay(100);
}

void loop() {
  servo.write(90); // 90도는 Duty Cycle 7.5%
  delay(1000);
}