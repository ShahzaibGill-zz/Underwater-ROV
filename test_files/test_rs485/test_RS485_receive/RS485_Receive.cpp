#include "RS485_Receive.h"
#include <Arduino.h>

RS485_Receive::RS485_Receive(void): RS485Serial(SSerialRX, SSerialTX){};

// Initializes a PID Controller with setpoint(desired value), agressive tuning parameters, and conservative tuning parameters.
void RS485_Receive::Initialize(){ 
  RS485Serial.begin(4800);   // set the data rate
  leftMotorValue = 1500;
  rightMotorValue = 1500;
  frontMotorValue = 1500;
  backMotorValue = 1500;
};


int RS485_Receive::GetMotorValue(char allMotorValues[], int index){
  char motorValue[4];
  for (int i = 0; i < 4; i++){
    motorValue[i] = allMotorValues[index++];
  };
  return atoi(motorValue);
}


void RS485_Receive::ReadPS3Values(){
  if (RS485Serial.available())
  {
    charReceived = RS485Serial.read();
    if (charReceived == CLOSING)
    {
      READ_MOTOR_VALUES = false;
      PARSE_VALUES = true;
    }  
    if (READ_MOTOR_VALUES) {
      motorValues[index++] = charReceived;
    }
    if (PARSE_VALUES) {
      leftMotorValue = GetMotorValue(motorValues, LEFT_MOTOR_INDEX);
      rightMotorValue = GetMotorValue(motorValues, RIGHT_MOTOR_INDEX);
      frontMotorValue = GetMotorValue(motorValues, FRONT_MOTOR_INDEX);
      backMotorValue = GetMotorValue(motorValues, BACK_MOTOR_INDEX);
      PARSE_VALUES = false;
      index = 0;
    }
    else if (!PARSE_VALUES && charReceived == IGNORE){
      leftMotorValue = STOP_MOTOR;
      rightMotorValue = STOP_MOTOR;
      frontMotorValue = STOP_MOTOR;
      backMotorValue = STOP_MOTOR;
    }
    if (charReceived == OPENING)
    {
      READ_MOTOR_VALUES = true;
    }
  }
}


void RS485_Receive::PrintMotorValues(int left,int right, int front, int back){
    Serial.print(F("\r\n: left "));
    Serial.print(left);
    Serial.print(F("\r: right "));
    Serial.print(right);
    Serial.print(F("\r: front "));
    Serial.print(front);
    Serial.print(F("\r: back"));
    Serial.print(back);
}
