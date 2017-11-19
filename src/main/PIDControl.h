#ifndef PIDControl_h
#define PIDControl_h

#include <PID_v1.h>

class PIDControl {
  public:
  
  PID PIDController;
  double Setpoint, Input, Output;
  //Aggressive and conservative Tuning Parameters
  double aggKp, aggKi, aggKd;
  double consKp, consKi, consKd;
  int dir;
  
  PIDControl(void);
  void Initialize(double _setpoint, double _aggKp, double _aggKi, double _aggKD, double _consKp, double _consKi, double _consKd);
  double GetPIDControlValue(double Input);
};

#endif
