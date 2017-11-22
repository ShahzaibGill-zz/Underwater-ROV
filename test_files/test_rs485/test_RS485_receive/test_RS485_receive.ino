/*-----( Import needed libraries )-----*/
#include <SoftwareSerial.h>
#include <Servo.h>
/*-----( Declare Constants and Pin Numbers )-----*/
#define SSerialRX        10  //Serial Receive pin
#define SSerialTX        11  //Serial Transmit pin
#define LEFT_M_PIN             8
#define RIGHT_M_PIN            11
#define FRONT_M_PIN            12
#define BACK_M_PIN             13

// Motor Threshold
#define FULL_CCW_THROT         1100
#define FULL_CW_THROT          1900

/*-----( Declare objects )-----*/
SoftwareSerial RS485Serial(SSerialRX, SSerialTX); // RX, TX

/*-----( Declare Variables )-----*/

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


char charReceived;
char motorValues[17];
int index = 0;
int backMotorValue;
int frontMotorValue;
int rightMotorValue;
int leftMotorValue;


void setup()
{
  Serial.begin(9600);
  Serial.println("SerialRemote");  // Can be ignored

  // Start the software serial port, to another device
  RS485Serial.begin(4800);   // set the data rate
  RIGHT_MOTOR.attach(RIGHT_M_PIN);
  LEFT_MOTOR.attach(LEFT_M_PIN);
  FRONT_MOTOR.attach(FRONT_M_PIN);
  BACK_MOTOR.attach(BACK_M_PIN);
  writeToMotors(STOP_MOTOR, STOP_MOTOR, STOP_MOTOR, STOP_MOTOR);
  delay(500);
}


void loop()
{
  if (RS485Serial.available())
  {
    charReceived = RS485Serial.read();
//    Serial.write(charReceived);
    if (charReceived == CLOSING)
    {
      READ_MOTOR_VALUES = false;
      PARSE_VALUES = true;
    }  
    if (READ_MOTOR_VALUES) {
      motorValues[index++] = charReceived;
    }
    if (PARSE_VALUES) {
//      Serial.println(motorValues);
      leftMotorValue = getMotorValue(motorValues, LEFT_MOTOR_INDEX);
      rightMotorValue = getMotorValue(motorValues, RIGHT_MOTOR_INDEX);
      frontMotorValue = getMotorValue(motorValues, FRONT_MOTOR_INDEX);
      backMotorValue = getMotorValue(motorValues, BACK_MOTOR_INDEX);
      PARSE_VALUES = false;
      index = 0;
    }
    else if (!PARSE_VALUES && charReceived == IGNORE){
      leftMotorValue = STOP_MOTOR;
      rightMotorValue = STOP_MOTOR;
      frontMotorValue = STOP_MOTOR;
      backMotorValue = STOP_MOTOR;
    }
    if (charReceived == OPENING)
    {
      READ_MOTOR_VALUES = true;
    }
//    print_motor_values(leftMotorValue, rightMotorValue, frontMotorValue, backMotorValue);
    Serial.println(leftMotorValue);
    writeToMotors(leftMotorValue, leftMotorValue, leftMotorValue, leftMotorValue);
//    Serial.println(leftMotorValue);
  }
//  else 
//  {
//    Serial.print("RS485 problem");
//  } 
}

/* Given character array of all motor values, returns integer of a particular motor value */
int getMotorValue(char allMotorValues[], int index){
  char motorValue[4];
  for (int i = 0; i < 4; i++){
    motorValue[i] = allMotorValues[index++];
  };
  return atoi(motorValue);
}

void writeToMotors(int leftMotorValue, int rightMotorValue, int frontMotorValue, int backMotorValue){
  RIGHT_MOTOR.writeMicroseconds(leftMotorValue);
  LEFT_MOTOR.writeMicroseconds(rightMotorValue);
  FRONT_MOTOR.writeMicroseconds(frontMotorValue);
  BACK_MOTOR.writeMicroseconds(backMotorValue);
}


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
