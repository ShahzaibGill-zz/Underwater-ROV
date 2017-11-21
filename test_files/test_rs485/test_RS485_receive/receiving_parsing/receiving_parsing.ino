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
String MOTOR_VALUES;
bool VALUES_READY = false;
bool PARSE_VALUES = false;
char OPENING = '(';
char CLOSING = ')';
int final_motor_values;
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
    Serial.write("hello");
    byteReceived = RS485Serial.read();   // Read the byte
    Serial.write(byteReceived);
    if (byteReceived == OPENING)
    {
      VALUES_READY = true;
    }
    if (byteReceived == CLOSING)
    {
      VALUES_READY = false;
      PARSE_VALUES = true;
    }
    if (VALUES_READY) {
      MOTOR_VALUES == (MOTOR_VALUES + String(byteReceived));
    }
    if (PARSE_VALUES) {
      final_motor_values = MOTOR_VALUES.toInt();
      back_motor = final_motor_values % 10000;
      final_motor_values = final_motor_values/ 10000;
      front_motor = final_motor_values % 10000;
      final_motor_values = final_motor_values/ 10000;
      right_motor = final_motor_values % 10000;
      final_motor_values = final_motor_values/ 10000;
      left_motor = final_motor_values/ 10000;
    }
    Serial.write(left_motor);   // Show on Serial Monitor

  }// End If RS485SerialAvailable

}//--(end main loop )---
