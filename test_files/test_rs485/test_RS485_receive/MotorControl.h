#ifndef Motor_Control_h
#define Motor_Control_h

#include <Servo.h>

class MotorControl {
  private:
  const int STOP_MOTOR = 1500;
  const int LEFT_M_PIN = 8;
  const int RIGHT_M_PIN = 11;
  const int FRONT_M_PIN = 12;
  const int BACK_M_PIN = 13;
  // Motor Threshold
  const int FULL_CCW_THROT = 1100;
  const int FULL_CW_THROT = 1900;
  
  public:
  Servo RIGHT_MOTOR, LEFT_MOTOR, BACK_MOTOR, FRONT_MOTOR;

  MotorControl(void);
  void Initialize();
  void Calibrate();
  void WriteToMotors(int leftMotorValue, int rightMotorValue, int frontMotorValue, int backMotorValue);
};

#endif

