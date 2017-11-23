#ifndef Motor_Control_h
#define Motor_Control_h

#include <Servo.h>

class MotorControl {
  private:
  const int LEFT_M_PIN = 8;
  const int RIGHT_M_PIN = 12;
  const int FRONT_M_PIN = 11;
  const int BACK_M_PIN = 9;
  // Motor Threshold
  const int STOP_MOTOR_LEFT = 1500;
  const int STOP_MOTOR_RIGHT = 1500;
  const int STOP_MOTOR_FRONT= 1550;
  const int STOP_MOTOR_BACK = 1500;
  const int FULL_CCW_THROT = 1060;
  const int FULL_CW_THROT = 1860;
  
  public:
  Servo RIGHT_MOTOR, LEFT_MOTOR, BACK_MOTOR, FRONT_MOTOR;

  MotorControl(void);
  void Initialize();
  void Calibrate();
  void WriteToMotors(int leftMotorValue, int backMotorValue, int frontMotorValue, int rightMotorValue);
  void PrintMotorValues(int left,int right, int front, int back);
};

#endif

