#include "MotorControl.h"
#include <Arduino.h>

MotorControl::MotorControl(void){};

void MotorControl::Initialize(){
  RIGHT_MOTOR.attach(RIGHT_M_PIN);
  LEFT_MOTOR.attach(LEFT_M_PIN);
  FRONT_MOTOR.attach(FRONT_M_PIN);
  BACK_MOTOR.attach(BACK_M_PIN);
  WriteToMotors(STOP_MOTOR, STOP_MOTOR, STOP_MOTOR, STOP_MOTOR);
};

void MotorControl::Calibrate(){
  WriteToMotors(FULL_CW_THROT,FULL_CW_THROT,FULL_CW_THROT,FULL_CW_THROT);
  delay(2000);
  WriteToMotors(STOP_MOTOR,STOP_MOTOR,STOP_MOTOR,STOP_MOTOR);
  delay(1000);
  WriteToMotors(FULL_CCW_THROT,FULL_CCW_THROT,FULL_CCW_THROT,FULL_CCW_THROT);
  delay(2000);
  WriteToMotors(STOP_MOTOR,STOP_MOTOR,STOP_MOTOR,STOP_MOTOR);
  };

void MotorControl::WriteToMotors(int leftMotorValue, int rightMotorValue, int frontMotorValue, int backMotorValue){
  RIGHT_MOTOR.writeMicroseconds(leftMotorValue);
  LEFT_MOTOR.writeMicroseconds(rightMotorValue);
  FRONT_MOTOR.writeMicroseconds(frontMotorValue);
  BACK_MOTOR.writeMicroseconds(backMotorValue);
}

