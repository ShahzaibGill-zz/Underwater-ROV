#include "IMU.h"

/* Constructor: IMU Sensor Initialization */
IMU::IMU(void){};

void IMU::Initialize(void){
  if(!accel.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println(F("Ooops, no LSM303 detected ... Check your wiring!"));
    while(1);
  }
  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  if(!bmp.begin())
  {
    /* There was a problem detecting the BMP180 ... check your connections */
    Serial.println("Ooops, no BMP180 detected ... Check your wiring!");
    while(1);
  }
}

void IMU::MeasureIMU(void){
  /* Calculate pitch and roll from the raw accelerometer data */
  accel.getEvent(&accel_event);
  dof.accelGetOrientation(&accel_event, &orientation);
  
  /* Calculate the heading using the magnetometer */
  mag.getEvent(&mag_event);
  dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation);
}


/* Determine IMU Pitch */
float IMU::getPitch(void){
  return orientation.pitch;
};

///* Determine IMU Yaw */
float IMU::getYaw(void){
  return orientation.heading;
};

///* Determine IMU Roll */
float IMU::getRoll(void){
  return orientation.roll;
}
