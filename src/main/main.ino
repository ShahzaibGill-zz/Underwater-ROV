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
 setpoint (desired value), agressive,conservative tuning parameters for PitchPID
double pitch_setpoint = 0, pitch_aggKp = 4, pitch_aggKi = 0, pitch_aggKd = 1, pitch_consKp = 2, pitch_consKi = 0, pitch_consKd = 0.5;


void setup() {
  Serial.begin(9600);  
  imu.Initialize();
  receiver.Initialize();
  PitchPID.Initialize(pitch_setpoint,pitch_aggKp,pitch_aggKi,pitch_aggKd,pitch_consKp,pitch_consKi,pitch_consKd);
  motorControl.Initialize();
  Serial.println("Initialization Complete");
  //delay(1000);
}

void loop() {

/* Reading motor values from PS3 Controller*/
  receiver.ReadPS3Values();
   motorControl.WriteToMotors(receiver.leftMotorValue,receiver.backMotorValue,receiver.frontMotorValue,receiver.rightMotorValue);

/* Get pitch, yaw, roll measurements from IMU */
  imu.MeasureIMU();
  pitch = imu.getPitch();
  Serial.print(F("Pitch: "));
  Serial.println(pitch);

/* Pitch PID Control Example - Parameter values may need to be tuned */
  int pitchControlValue = PitchPID.GetPIDControlValue(pitch);
  Serial.println(pitchControlValue);
  motorControl.WriteToMotors(1500,1500+pitchControlValue,1550,1500);
  motorControl.WriteToMotors(receiver.leftMotorValue,receiver.backMotorValue+pitchControlValue,receiver.frontMotorValue-pitchControlValue,receiver.rightMotorValue);
}
