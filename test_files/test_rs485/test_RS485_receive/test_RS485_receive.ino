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
int byteReceived;
int byteSend;

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
    Serial.write(byteReceived);   // Show on Serial Monitor

  }// End If RS485SerialAvailable

}//--(end main loop )---
