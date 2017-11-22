#include "RS485_Receive.h"

RS485_Receive receiver;

void setup()
{
  Serial.begin(9600);
  receiver.Initialize();
  Serial.println("SerialRemote");  // Can be ignored  
}


void loop()
{
  receiver.ReadPS3Values();
  Serial.print(receiver.leftMotorValue);
  
}

//void writeToMotors(int leftMotorValue, int rightMotorValue, int frontMotorValue, int backMotorValue){
//  RIGHT_MOTOR.writeMicroseconds(leftMotorValue);
//  LEFT_MOTOR.writeMicroseconds(rightMotorValue);
//  FRONT_MOTOR.writeMicroseconds(frontMotorValue);
//  BACK_MOTOR.writeMicroseconds(backMotorValue);
//}


