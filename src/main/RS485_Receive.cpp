#include "RS485_Receive.h"
#include <Arduino.h>

RS485_Receive::RS485_Receive(void): RS485Serial(SSerialRX, SSerialTX){};

// Initializes a PID Controller with setpoint(desired value), agressive tuning parameters, and conservative tuning parameters.
void RS485_Receive::Initialize(){ 
  RS485Serial.begin(4800);   // set the data rate
  leftMotorValue = STOP_MOTOR_LEFT;
  rightMotorValue = STOP_MOTOR_RIGHT;
  frontMotorValue = STOP_MOTOR_FRONT;
  backMotorValue = STOP_MOTOR_BACK;
};


int RS485_Receive::GetMotorValue(char allMotorValues[], int index){
  char motorValue[5];
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
      String motorValuesString = motorValues;
//      leftMotorValue = GetMotorValue(motorValues, LEFT_MOTOR_INDEX);
//      rightMotorValue = GetMotorValue(motorValues, RIGHT_MOTOR_INDEX);
//      frontMotorValue = GetMotorValue(motorValues, FRONT_MOTOR_INDEX);
//      backMotorValue = GetMotorValue(motorValues, BACK_MOTOR_INDEX);
      leftMotorValue = motorValuesString.substring(0,4).toInt();
      rightMotorValue = motorValuesString.substring(4,8).toInt();
      frontMotorValue = motorValuesString.substring(8,12).toInt();
      backMotorValue = motorValuesString.substring(12,16).toInt();
      PARSE_VALUES = false;
      index = 0;
    }
    else if (!PARSE_VALUES && charReceived == IGNORE){
      leftMotorValue = STOP_MOTOR_LEFT;
      rightMotorValue = STOP_MOTOR_RIGHT;
      frontMotorValue = STOP_MOTOR_FRONT;
      backMotorValue = STOP_MOTOR_BACK;
    }
    if (charReceived == OPENING)
    {
      READ_MOTOR_VALUES = true;
    }
  }
}

