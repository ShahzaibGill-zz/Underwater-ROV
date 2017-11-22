#include "Arduino.h"
#include "IMU.h"
#include "PIDControl.h"
#include "MotorControl.h"
#include "RS485_Receive.h"

/*Variable initialization */
IMU imu;
RS485_Receive receiver;
MotorControl motorControl;
double roll,yaw,pitch;


/* PID variable initialization */
PIDControl PitchPID;
// setpoint (desired value), agressive,conservative tuning parameters for PitchPID
double pitch_setpoint = 0, pitch_aggKp = 4, pitch_aggKi = 0, pitch_aggKd = 1, pitch_consKp = 1, pitch_consKi = 0, pitch_consKd = 0.25;


void setup() {
  Serial.begin(9600);
//  imu.Initialize();
  receiver.Initialize();
//  PitchPID.Initialize(pitch_setpoint,pitch_aggKp,pitch_aggKi,pitch_aggKd,pitch_consKp,pitch_consKi,pitch_consKd);
  motorControl.Initialize();
  delay(1000);
  motorControl.Calibrate();
}

void loop() {
  receiver.ReadPS3Values();
  Serial.print(receiver.leftMotorValue);
//  imu.MeasureIMU();
//  pitch = imu.getPitch();
//  Serial.print(F("Pitch: "));
//  Serial.println(pitch);
//  Serial.println(PitchPID.GetPIDControlValue(pitch));
//  delay(1000);
}
