#include <Servo.h>
#include <Wire.h>

Servo MOTOR1;
Servo MOTOR2;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  MOTOR1.attach(9);
//  MOTOR2.attach(11);
  delay(4000);

}

void loop() {
  // put your main code here, to run repeatedly:
  run_motors(1300,1300);
  delay(4000);
  run_motors(1500,1500);
  delay(4000);
  run_motors(1600,1600);
  delay(4000);
  run_motors(1500,1500);
  delay(10000);
}


void run_motors(int m1val, int m2val)
{
  MOTOR1.write(m1val);
//  MOTOR2.writeMicroseconds(m2val);

}
