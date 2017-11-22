/*-----( Import needed libraries )-----*/
#include <SoftwareSerial.h>
#include <Servo.h>
#include "RS485_Receive.h"

RS485_Receive receiver;

void setup()
{
  Serial.begin(9600);
  Serial.println("SerialRemote");  // Can be ignored
  receiver.Initialize();
  // Start the software serial port, to another device
  //  writeToMotors(STOP_MOTOR, STOP_MOTOR, STOP_MOTOR, STOP_MOTOR);
  delay(500);
}


void loop()
{
  receiver.readPS3Values();
  print_motor_values(receiver.leftMotorValue, receiver.rightMotorValue, receiver.frontMotorValue, receiver.backMotorValue);
// getMotorValue();
}

/* Given character array of all motor values, returns integer of a particular motor value */
int getMotorValue(char allMotorValues[], int index){
  char motorValue[4];
  for (int i = 0; i < 4; i++){
    motorValue[i] = allMotorValues[index++];
  };
  return atoi(motorValue);
}

//void writeToMotors(int leftMotorValue, int rightMotorValue, int frontMotorValue, int backMotorValue){
//  RIGHT_MOTOR.writeMicroseconds(leftMotorValue);
//  LEFT_MOTOR.writeMicroseconds(rightMotorValue);
//  FRONT_MOTOR.writeMicroseconds(frontMotorValue);
//  BACK_MOTOR.writeMicroseconds(backMotorValue);
//}


void print_motor_values(int left,int right, int front, int back){
    Serial.print(F("\r\n: left "));
    Serial.print(left);
    Serial.print(F("\r: right "));
    Serial.print(right);
    Serial.print(F("\r: front "));
    Serial.print(front);
    Serial.print(F("\r: back"));
    Serial.print(back);
}
