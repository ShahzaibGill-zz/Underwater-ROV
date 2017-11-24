#include "RS485_Receive.h"
#include "MotorControl.h"

RS485_Receive receiver;
MotorControl motorControl;

void setup()
{
  Serial.begin(9600);
  receiver.Initialize();
  Serial.println("SerialRemote");  // Can be ignored  
  motorControl.Initialize();
}


void loop()
{
  receiver.ReadPS3Values();
  motorControl.WriteToMotors(1700,1700,1700,1700);
  Serial.print(receiver.leftMotorValue);
  
}

//void writeToMotors(int leftMotorValue, int rightMotorValue, int frontMotorValue, int backMotorValue){
//  RIGHT_MOTOR.writeMicroseconds(leftMotorValue);
//  LEFT_MOTOR.writeMicroseconds(rightMotorValue);
//  FRONT_MOTOR.writeMicroseconds(frontMotorValue);
//  BACK_MOTOR.writeMicroseconds(backMotorValue);
//}


