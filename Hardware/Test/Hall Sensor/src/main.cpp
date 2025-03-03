#include <Arduino.h>

int sensor = A2;

void setup() {
    Serial.begin(9600);
}

void loop() {
    int val = analogRead(sensor);
    Serial.print("Sensor Value: ");
    Serial.println(val);
    
    delay(100);
}