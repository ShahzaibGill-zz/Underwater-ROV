#ifndef RS485_Receive_h
#define RS485_Receive_h

#include <SoftwareSerial.h>

class RS485_Receive {
  private:
  const int SSerialRX = 10;  //Serial Receive pin
  const int SSerialTX = 11;  //Serial Transmit pin
  const int STOP_MOTOR_LEFT = 1500;
  const int STOP_MOTOR_RIGHT = 1500;
  const int STOP_MOTOR_FRONT= 1550;
  const int STOP_MOTOR_BACK = 1500;
  const int LEFT_MOTOR_INDEX = 0;
  const int RIGHT_MOTOR_INDEX = 4;
  const int FRONT_MOTOR_INDEX = 8;
  const int BACK_MOTOR_INDEX = 12;
  const int STOP_MOTOR= 1500;
  const char OPENING = '(';
  const char CLOSING = ')';
  const char IGNORE = '*';
  bool READ_MOTOR_VALUES = false;
  bool PARSE_VALUES = false;
  
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
  int GetMotorValue(char*, int);
  void ReadPS3Values();
  void PrintMotorValues(int left,int right, int front, int back);
};

#endif

