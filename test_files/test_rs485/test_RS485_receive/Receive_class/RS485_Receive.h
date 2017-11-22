#ifndef RS485_Receive_h
#define RS485_Receive_h

#include <SoftwareSerial.h>
#include <Servo.h>

class RS485_Receive {
  private:
  const int SSerialRX = 10;  //Serial Receive pin
  const int SSerialTX = 11;  //Serial Transmit pin
  const int LEFT_M_PIN = 8;
  const int RIGHT_M_PIN = 11;
  const int FRONT_M_PIN = 12;
  const int BACK_M_PIN = 13;
  const int STOP_MOTOR = 1500;
  const int LEFT_MOTOR_INDEX = 0;
  const int RIGHT_MOTOR_INDEX = 4;
  const int FRONT_MOTOR_INDEX = 8;
  const int BACK_MOTOR_INDEX = 12;
  const char OPENING = '(';
  const char CLOSING = ')';
  const char IGNORE = '*';
  bool READ_MOTOR_VALUES = false;
  bool PARSE_VALUES = false;
  Servo RIGHT_MOTOR, LEFT_MOTOR, BACK_MOTOR, FRONT_MOTOR;
  
  public:
  SoftwareSerial RS485Serial; // RX, TX
  char charReceived;
  char motorValues[17];
  int index = 0;
  int backMotorValue;
  int frontMotorValue;
  int rightMotorValue;
  int leftMotorValue;

  RS485_Receive(void);
  void Initialize();
  int getMotorValue(char*, int);
  void readPS3Values();
};

#endif

