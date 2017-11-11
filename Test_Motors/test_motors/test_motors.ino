#include <Servo.h>
#include <Wire.h>

Servo MOTOR1;
Servo MOTOR2;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  MOTOR1.attach(12);
  MOTOR2.attach(11);

}

void loop() {
  // put your main code here, to run repeatedly:
  run_motors(150,150);
  delay(4000);
  run_motors(90,90);
  delay(4000);
  run_motors(30,30);
  delay(4000);
  run_motors(90,90);
}


void run_motors(int m1val, int m2val)
{
  MOTOR1.write(m1val);
  MOTOR2.write(m2val);

}
