#include "MotorControl.h"
#include <Arduino.h>

MotorControl::MotorControl(void){};

void MotorControl::Initialize(){
  RIGHT_MOTOR.attach(RIGHT_M_PIN);
  LEFT_MOTOR.attach(LEFT_M_PIN);
  FRONT_MOTOR.attach(FRONT_M_PIN);
  BACK_MOTOR.attach(BACK_M_PIN);
  WriteToMotors(STOP_MOTOR_LEFT,STOP_MOTOR_RIGHT,STOP_MOTOR_FRONT,STOP_MOTOR_BACK);
};

void MotorControl::Calibrate(){
  // Attach motors
  RIGHT_MOTOR.attach(RIGHT_M_PIN, 1060, 1860);
  LEFT_MOTOR.attach(LEFT_M_PIN, 1060, 1860);
  FRONT_MOTOR.attach(FRONT_M_PIN, 1060, 1860);
  BACK_MOTOR.attach(BACK_M_PIN, 1060, 1860);
  // 
  WriteToMotors(FULL_CW_THROT,FULL_CW_THROT,FULL_CW_THROT,FULL_CW_THROT);
  Serial.print("Plug in battery now");
  delay(5000);
  WriteToMotors(FULL_CCW_THROT,FULL_CCW_THROT,FULL_CCW_THROT,FULL_CCW_THROT);
  delay(2000);
  WriteToMotors(FULL_CCW_THROT,FULL_CCW_THROT,FULL_CCW_THROT,FULL_CCW_THROT);
  delay(2000);
  WriteToMotors(STOP_MOTOR_LEFT,STOP_MOTOR_RIGHT,STOP_MOTOR_FRONT,STOP_MOTOR_BACK);
  };

void MotorControl::WriteToMotors(int leftMotorValue, int backMotorValue, int frontMotorValue, int rightMotorValue){
  RIGHT_MOTOR.writeMicroseconds(rightMotorValue);
  LEFT_MOTOR.writeMicroseconds(leftMotorValue);
  FRONT_MOTOR.writeMicroseconds(frontMotorValue);
  BACK_MOTOR.writeMicroseconds(backMotorValue);
}

void MotorControl::PrintMotorValues(int left, int back, int front, int right){
    Serial.print(F("\r\n: left "));
    Serial.print(left);
    Serial.print(F("\r: right "));
    Serial.print(right);
    Serial.print(F("\r: front "));
    Serial.print(front);
    Serial.print(F("\r: back"));
    Serial.print(back);
}

