#include <Servo.h>
#include <Wire.h>

Servo MOTOR1;
Servo MOTOR2;
Servo MOTOR3;
Servo MOTOR4;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  MOTOR1.attach(8);
  MOTOR2.attach(9);
  MOTOR3.attach(11);
  MOTOR4.attach(12);
  delay(2000);

}

void loop() {
  // put your main code here, to run repeatedly:
  run_motors(1800,1800,1800,1800);
//  delay(2000);
//  run_motors(1500,1500,1500,1500);
//  delay(2000);
//  run_motors(1700,1700,1700,1700);
//  delay(2000);
//  run_motors(1500,1500,1500,1500);
//  delay(10000);
}


void run_motors(int m1val, int m2val, int m3val, int m4val)
{
  MOTOR1.writeMicroseconds(m1val);
  MOTOR2.writeMicroseconds(m2val);
  MOTOR3.writeMicroseconds(m3val);
  MOTOR4.writeMicroseconds(m4val);

}
