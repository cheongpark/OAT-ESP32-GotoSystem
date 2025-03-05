#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

class MPU6050 {
public:
  struct Acceleration {
    float x;
    float y;
    float z;
  };

  struct Gyroscope {
    float x;
    float y;
    float z;
  };

  struct Temperature {
    float temperature;
  };
  
  Acceleration acceleration;
  Gyroscope gyroscope;
  Temperature temperature;
};

class MPUSensor {
public:
  MPUSensor();

  bool read(MPU6050 &data);

private:
  Adafruit_MPU6050 mpu;
};

MPUSensor::MPUSensor() {
  if (!mpu.begin()) {
    Serial.println("MPU6050 Connected Failed");

    while (true)
      delay(100);
  }

  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
}

bool MPUSensor::read(MPU6050 &data) {
  sensors_event_t accel, gyro, temp; 
  mpu.getEvent(&accel, &gyro, &temp);
  
  data.acceleration = { accel.acceleration.x, accel.acceleration.y, accel.acceleration.z };
  data.gyroscope = { gyro.gyro.x, gyro.gyro.y, gyro.gyro.z };  
  data.temperature = { temp.temperature };
  
  return true;
}