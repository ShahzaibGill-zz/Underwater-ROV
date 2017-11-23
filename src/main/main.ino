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
  motorControl.WriteToMotors(1500,1500,1550,1500);
  Serial.print("Starting");
  delay(5000);
//  motorControl.Calibrate();
}

void loop() {
  receiver.ReadPS3Values();
  Serial.println(receiver.frontMotorValue);
  motorControl.WriteToMotors(receiver.leftMotorValue,receiver.backMotorValue,receiver.frontMotorValue,receiver.rightMotorValue);
//  motorControl.WriteToMotors(1500,1500,1550,1500);
//   Left, Back, Right, Front
//   Fixed
//  delay(5000);
//  Serial.println("Run 1700");
//  motorControl.WriteToMotors(1700,1700,1700,1700);
//  delay(5000);
//  Serial.println("Run 1500");
//  motorControl.WriteToMotors(1500,1500,1550,1500);
//  delay(5000);
//  motorControl.WriteToMotors(1200,1200,1550,1200);
//  Serial.println("Run 1200");
//  delay(5000);
//  delay(3000);  
//  Serial.println(receiver.leftMotorValue);
//  Serial.print(receiver.backMotorValue);
//  motorControl.WriteToMotors(receiver.leftMotorValue,receiver.rightMotorValue,receiver.frontMotorValue,receiver.backMotorValue);
//  motorControl.PrintMotorValues(receiver.leftMotorValue,receiver.rightMotorValue,receiver.frontMotorValue,receiver.backMotorValue);
//  imu.MeasureIMU();
//  pitch = imu.getPitch();
//  Serial.print(F("Pitch: "));
//  Serial.println(pitch);
//  Serial.println(PitchPID.GetPIDControlValue(pitch));
//  delay(1000);
}
