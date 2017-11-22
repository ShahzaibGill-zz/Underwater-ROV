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


int RS485_Receive::getMotorValue(char allMotorValues[], int index){
  char motorValue[4];
  for (int i = 0; i < 4; i++){
    motorValue[i] = allMotorValues[index++];
  };
  return atoi(motorValue);
}


void RS485_Receive::readPS3Values(){
  if (RS485Serial.available())
  {
    charReceived = RS485Serial.read();
    if (charReceived == CLOSING) {
      READ_MOTOR_VALUES = false;
      PARSE_VALUES = true;
    }  
    if (READ_MOTOR_VALUES) {
      motorValues[index++] = charReceived;
    }
    if (PARSE_VALUES) {
      leftMotorValue = getMotorValue(motorValues, LEFT_MOTOR_INDEX);
      rightMotorValue = getMotorValue(motorValues, RIGHT_MOTOR_INDEX);
      frontMotorValue = getMotorValue(motorValues, FRONT_MOTOR_INDEX);
      backMotorValue = getMotorValue(motorValues, BACK_MOTOR_INDEX);
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

