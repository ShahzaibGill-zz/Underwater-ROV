#include "Arduino.h"
#include "IMU.h"

IMU imu;
float roll,yaw,pitch;

void setup() {
  Serial.begin(9600);
  imu.Initialize();
}

void loop() {
  imu.MeasureIMU();
  yaw = imu.getRoll();
  Serial.println(yaw);
}
