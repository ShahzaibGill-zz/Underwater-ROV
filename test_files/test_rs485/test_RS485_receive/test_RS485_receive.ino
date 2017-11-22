/*
   - Pin 10 -> RO
   - 5V, GND -> VCC, GND
   - Open Serial Monitor, type in top window.
   - Other Serial Monitor will display value sent
*/

/*-----( Import needed libraries )-----*/
#include <SoftwareSerial.h>
/*-----( Declare Constants and Pin Numbers )-----*/
#define SSerialRX        10  //Serial Receive pin
#define SSerialTX        11  //Serial Transmit pin

/*-----( Declare objects )-----*/
SoftwareSerial RS485Serial(SSerialRX, SSerialTX); // RX, TX

/*-----( Declare Variables )-----*/
char byteReceived;
int byteSend;
char MOTOR_VALUES[17];
int index = 0;
bool READ_MOTOR_VALUES = false;
bool PARSE_VALUES = false;
char OPENING = '(';
char CLOSING = ')';
long final_motor_values;
int back_motor;
int front_motor;
int right_motor;
int left_motor;


void setup()   /****** SETUP: RUNS ONCE ******/
{
  // Start the built-in serial port, probably to Serial Monitor
  Serial.begin(9600);
  Serial.println("SerialRemote");  // Can be ignored

  // Start the software serial port, to another device
  RS485Serial.begin(4800);   // set the data rate
}//--(end setup )---


void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{
  //Copy input data to output
  if (RS485Serial.available())
  {
    byteReceived = RS485Serial.read();   // Read the byte
    if (byteReceived == CLOSING)
    {
      READ_MOTOR_VALUES = false;
      PARSE_VALUES = true;
    }
    if (READ_MOTOR_VALUES) {
      MOTOR_VALUES[index++] = byteReceived;
//      Serial.print(MOTOR_VALUES);
    }
    if (PARSE_VALUES) {
      left_motor = getMotorValue(MOTOR_VALUES, 0);
      right_motor = getMotorValue(MOTOR_VALUES, 4);
      front_motor = getMotorValue(MOTOR_VALUES, 8);
      back_motor = getMotorValue(MOTOR_VALUES, 12);
      PARSE_VALUES = false;
      index = 0;
    }
//    Serial.write(left_motor);   // Show on Serial Monitor
    if (byteReceived == OPENING)
    {
      READ_MOTOR_VALUES = true;
    }
  }// End If RS485SerialAvailable
  
}//--(end main loop )---


int getMotorValue(char arr[], int index){
  char motor_value[5];
  for (int i = 0; i < 4; i++){
    motor_value[i] = arr[index++];
  };
  return atoi(motor_value);
}
