#include <PS3USB.h>
#include <SoftwareSerial.h>


// Pin decrelation
#define SSerialRX              10  //Serial Receive pin
#define SSerialTX              3  //Serial Transmit pin

// Motor Threshold
#define FULL_CCW_THROT         1300
#define FULL_CW_THROT          1600

// TODO - make this a range if we have a deadzone
#define FULL_STOP              1500
#define HALFPOINT              500


// Analog Threshold
#define NETURAL               128
#define FULL_DOWN             255
#define FULL_RIGHT            255
#define NOT_USED_SCALE1       100 // only use values below not used scale
#define NOT_USED_SCALE2       150 // only use values not not used scale

// L2 and R1 Threshold
#define MAX_PRESSED            255
#define L2_THRESHOLD           30
#define R2_THRESHOLD           30

const int FULL_STOP_LEFT=1500;
const int FULL_STOP_BACK=1500;
const int FULL_STOP_FRONT=1500;
const int FULL_STOP_RIGHT=1500;
const float THRUST_INTERVAL_SCALE_DOWN = 1.4;
const float TURN_INTERVAL_SCALE_DOWN = 1.3;
const float ANALOG_THROT_INTERVAL = float(1000)/(float)(2*FULL_DOWN*THRUST_INTERVAL_SCALE_DOWN); // 1000 - range of motor write values
const float TURN_SCALING_INTERVAL = float(1)/float(2*FULL_RIGHT*TURN_INTERVAL_SCALE_DOWN);
const float ELEVATION_SCALING_INTERVAL = float(1000)/float(2*MAX_PRESSED*THRUST_INTERVAL_SCALE_DOWN); //1000 - range of motor write values

USB Usb;
PS3USB PS3(&Usb); // Create an instance of PS3
SoftwareSerial RS485Serial(SSerialRX, SSerialTX); // RX, TX

int left_throt, right_throt, front_throt, back_throt; //Throttle applied to each motors
String write_to_RS_Serial;


//printing/testing functions 
//void print_motor_values(int left, int right, int front, int back);
void write_to_motor();

// ROV motion functions
void move_fwd();
void move_bkwd();
void move_up();
void move_down();

// communication handlers
void send_to_mega();

void setup()   /****** SETUP: RUNS ONCE ******/
{
  // Start the built-in serial port, probably to Serial Monitor
  Serial.begin(9600);
  Serial.println("YourDuino.com SoftwareSerial remote loop example");
  Serial.println("Use Serial Monitor, type in upper window, ENTER");

  // Start the software serial port, to another device
  RS485Serial.begin(4800);   // set the data rate
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1);
  }
  Serial.print(F("\r\nPS3 USB Library Started"));
}//--(end setup )---


void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{
  Usb.Task();
  // Get analog values to move and steer
  if(PS3.getAnalogHat(LeftHatY)  < NOT_USED_SCALE1){ // FWD case
    move_fwd();
  } else if(PS3.getAnalogHat(LeftHatY) > NOT_USED_SCALE2){ // BKWD case
     move_bkwd();
  }else{
     right_throt = FULL_STOP_RIGHT;
     left_throt = FULL_STOP_LEFT;
  }
  // Use R2 and l2 to move up and down
  if(PS3.getAnalogButton(L2) > L2_THRESHOLD  && PS3.getAnalogButton(R2) < R2_THRESHOLD){
    move_down();
  }else if(PS3.getAnalogButton(L2) < L2_THRESHOLD  && PS3.getAnalogButton(R2) > R2_THRESHOLD){
    move_up();
  }
  else{
    front_throt = FULL_STOP_FRONT;
    back_throt = FULL_STOP_BACK;
  }
  
  print_motor_values(left_throt, right_throt, front_throt, back_throt);
////   write_to_motor();
  send_to_mega();
  delay(200);
//  if (Serial.available())
//  {
//    String byteReceived= "(1500200025003000)";
//    int byteReceived = Serial.read()    ;
//    RS485Serial.write(byteReceived.c_str());          // Send byte to Remote Arduino
//  }


}//--(end main loop )---


// ---------------------- HELPER functions -  communication ---------------------------//
void send_to_mega(){
  if(left_throt == FULL_STOP_LEFT && right_throt == FULL_STOP_RIGHT && front_throt == FULL_STOP_FRONT &&  back_throt == FULL_STOP_BACK){
    write_to_RS_Serial = String("*");
  } else{
    write_to_RS_Serial = String("(" +String(left_throt) + String(right_throt) + String(front_throt) + String(back_throt) + ")");
  }

//  Serial.write(write_to_RS_Serial.c_str());
  RS485Serial.write(write_to_RS_Serial.c_str());          // Send byte to Remote Arduino
//  Serial.write(test_string.c_str());
//  delay(100);
}

// ---------------------- HELPER functions -  motion ---------------------------//

void move_fwd(){
  if(PS3.getAnalogHat(LeftHatY) < NOT_USED_SCALE1){ // check FWD motion in f'ns as well
    // 128 -> 0 map to 1500 -> 2000 motr values - Use FULL_DOWN(255) as refernce point
//    right_throt = (float)FULL_STOP + ((float)FULL_DOWN - (float)PS3.getAnalogHat(LeftHatY))* ANALOG_THROT_INTERVAL;
//    left_throt = (float)FULL_STOP + ((float)FULL_DOWN -  (float)PS3.getAnalogHat(LeftHatY))* ANALOG_THROT_INTERVAL;
      right_throt = FULL_CW_THROT;
      left_throt = FULL_CW_THROT;
    if(PS3.getAnalogHat(RightHatX) < NOT_USED_SCALE1){ // left turn
        // same logic as FWD motion - 128 -> 0 map to change in throttle 
//        left_throt = left_throt*(1.00 - ((float)FULL_RIGHT - (float)PS3.getAnalogHat(RightHatX))*TURN_SCALING_INTERVAL*0.5);
        left_throt = FULL_STOP_LEFT;
    } else if(PS3.getAnalogHat(RightHatX) > NOT_USED_SCALE2){ // right turn
//        right_throt = right_throt*(1.00 - (float)PS3.getAnalogHat(RightHatX)*TURN_SCALING_INTERVAL*0.5);
          right_throt = FULL_STOP_RIGHT;
    }
  }
}

// Inverse FWD logic
void move_bkwd(){
  if(PS3.getAnalogHat(LeftHatY) > NOT_USED_SCALE1){ //check BKWD motion in f'ns as well
//    right_throt = (float)FULL_STOP - (float)PS3.getAnalogHat(LeftHatY)* ANALOG_THROT_INTERVAL;
//    left_throt = (float)FULL_STOP - (float)PS3.getAnalogHat(LeftHatY)* ANALOG_THROT_INTERVAL;
      right_throt = FULL_CCW_THROT;
      left_throt = FULL_CCW_THROT;
    if(PS3.getAnalogHat(RightHatX) < NOT_USED_SCALE1){ // left turn
//        left_throt = left_throt*(1.00 + ((float)FULL_RIGHT - (float)PS3.getAnalogHat(RightHatX))*TURN_SCALING_INTERVAL);
      left_throt = FULL_STOP_LEFT;
    } else if(PS3.getAnalogHat(RightHatX) > NOT_USED_SCALE2){ // right turn
//        right_throt = right_throt*(1.00 + (float)PS3.getAnalogHat(RightHatX)*TURN_SCALING_INTERVAL);
      right_throt = FULL_STOP_RIGHT;
    }
  }
}

void move_up(){
//  front_throt = (float)FULL_STOP + (float)PS3.getAnalogButton(R2)* ELEVATION_SCALING_INTERVAL;
//  back_throt = (float)FULL_STOP +  (float)PS3.getAnalogButton(R2)* ELEVATION_SCALING_INTERVAL;
    front_throt = FULL_CW_THROT;
    back_throt = FULL_CW_THROT;
}

void move_down(){
//  front_throt = (float)FULL_STOP - (float)PS3.getAnalogButton(L2)* ELEVATION_SCALING_INTERVAL;
//  back_throt = (float)FULL_STOP - (float)PS3.getAnalogButton(L2)* ELEVATION_SCALING_INTERVAL;
    front_throt = FULL_CCW_THROT;
    back_throt = FULL_CCW_THROT;
}

// ---------------------- TESTING functions - remove in final design ---------------------------//

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

