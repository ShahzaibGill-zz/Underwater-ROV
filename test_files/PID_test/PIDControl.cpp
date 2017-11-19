#include "PIDControl.h"
#include <Arduino.h>

PIDControl::PIDControl(void): PIDController(&Input, &Output, &Setpoint, consKp, consKi, consKd, DIRECT){};

// Initializes a PID Controller with setpoint(desired value), agressive tuning parameters, and conservative tuning parameters.
void PIDControl::Initialize(double _setpoint, double _aggKp, double _aggKi, double _aggKD, double _consKp, double _consKi, double _consKd){ 
  Input = 0;
  Setpoint = _setpoint;
  aggKp=_aggKp, aggKi=_aggKi, aggKd=_aggKD;
  consKp=_consKp, consKi=_consKi, consKd=_consKd;
  PIDController.SetMode(AUTOMATIC);
  PIDController.SetOutputLimits(0,400);
  
};
double PIDControl::GetPIDControlValue(double ControlInput){
  Input = ControlInput;
  // If input is negative, corrective output should be positive 
  if (Input < 0){
    PIDController.SetControllerDirection(DIRECT);
    dir = 1;
  }
  else
  {
    // If input is positive, corrective output should be negative
    PIDController.SetControllerDirection(REVERSE);
    dir = -1;
  }
  double gap = abs(Setpoint-Input); //distance away from setpoint
  if(gap<10)
  {  //we're close to setpoint, use conservative tuning parameters
    PIDController.SetTunings(consKp, consKi, consKd);
  }
  else
  {
     //we're far from setpoint, use aggressive tuning parameters
     PIDController.SetTunings(aggKp, aggKi, aggKd);
  }
  PIDController.Compute();
  return dir*Output;
};
