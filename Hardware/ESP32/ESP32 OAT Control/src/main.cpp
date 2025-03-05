#include <Arduino.h>

const int hallpin = 5;
int val = 0;

void setup() {
  Serial.begin(115200);
}

void loop() {
  val = digitalRead(hallpin);
  Serial.println(val);
}