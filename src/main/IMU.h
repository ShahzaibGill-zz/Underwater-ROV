#ifndef IMU_h
#define IMU_h

#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_10DOF.h>

class IMU
{
private:
  Adafruit_10DOF                dof;
  Adafruit_LSM303_Accel_Unified accel;
  Adafruit_LSM303_Mag_Unified   mag;
  Adafruit_BMP085_Unified       bmp;
  sensors_event_t accel_event;
  sensors_event_t mag_event;
  sensors_event_t bmp_event;
  sensors_vec_t   orientation;
public:
  IMU(void);
  void Initialize();
  void MeasureIMU();
  float getPitch();
  float getRoll();
  float getYaw();
};

#endif
