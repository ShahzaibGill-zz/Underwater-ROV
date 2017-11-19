/******************************************************************
* motor treshold
 ******************************************************************/
#define FULL_CCW_THROT         1000
#define FULL_CW_THROT          2000

// TODO - make this a range if we have a deadzone
#define FULL_STOP              1500
#define HALFPOINT              500


/******************************************************************
* analog threshold
 ******************************************************************/
#define NETURAL               128
#define FULL_FWD              0
#define FULL_BKWD             255
#define FULL_LEFT             0
#define FULL_RIGHT            255
#define NOT_USED_SCALE1       100 // only use values below not used scale
#define NOT_USED_SCALE2       150 // only use values not not used scale


#include <PS3USB.h>
  

const float analong_throt_interval = float(1000)/(float)(2*FULL_BKWD);
const float turn_scaling_interval = float(1)/float(2*FULL_RIGHT);

USB Usb;
PS3USB PS3(&Usb); // Create an instance of PS3

float right_throt, left_throt;
int left_x, right_y;

void print_motor_values(int right,int left);
void move_fwd();
void move_bkwd();


void setup() {
  Serial.begin(115200);
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1);
  }
  Serial.print(F("\r\nPS3 USB Library Started"));
}


void loop() {
  Usb.Task();
  if(PS3.getAnalogHat(RightHatY)  < NOT_USED_SCALE1){ // FWD case
    move_fwd();
  } else if(PS3.getAnalogHat(RightHatY) > NOT_USED_SCALE2){ // BKWD case
     move_bkwd();
  }else{
     print_motor_values(FULL_STOP,FULL_STOP);
  }
  delay(500);
}


// ---------------------- HELPER functions for motion ---------------------------//
void print_motor_values(int right,int left){
    Serial.print(F("\r\n: wrting to left "));
    Serial.print(left);
    Serial.print(F("\r\n: wrting to right "));
    Serial.print(right);
}

void move_fwd(){
    if(PS3.getAnalogHat(RightHatY) < NOT_USED_SCALE1){
      right_throt = (float)FULL_STOP +((float)FULL_BKWD - (float)PS3.getAnalogHat(RightHatY))* analong_throt_interval;
      left_throt = (float)FULL_STOP +((float)FULL_BKWD -  (float)PS3.getAnalogHat(RightHatY))* analong_throt_interval;
    if(PS3.getAnalogHat(LeftHatX) < NOT_USED_SCALE1){ // left turn
        left_throt = left_throt*(1.00 - ((float)FULL_RIGHT - (float)PS3.getAnalogHat(LeftHatX))*turn_scaling_interval*0.5);
    } else if(PS3.getAnalogHat(LeftHatX) > NOT_USED_SCALE2){ // right turn
        right_throt = right_throt*(1.00 - (float)PS3.getAnalogHat(LeftHatX)*turn_scaling_interval*0.5);
  }
  print_motor_values(right_throt, left_throt); 
  }
}

 void move_bkwd(){
  if(PS3.getAnalogHat(RightHatY) > NOT_USED_SCALE1){
    right_throt = (float)FULL_STOP - (float)PS3.getAnalogHat(RightHatY)* analong_throt_interval;
    left_throt = (float)FULL_STOP - (float)PS3.getAnalogHat(RightHatY)* analong_throt_interval;
    if(PS3.getAnalogHat(LeftHatX) < NOT_USED_SCALE1){ // left turn
        left_throt = left_throt*(1.00 + ((float)FULL_RIGHT - (float)PS3.getAnalogHat(LeftHatX))*turn_scaling_interval);
    } else if(PS3.getAnalogHat(LeftHatX) > NOT_USED_SCALE2){ // right turn
        right_throt = right_throt*(1.00 + (float)PS3.getAnalogHat(LeftHatX)*turn_scaling_interval);
  }
  print_motor_values(right_throt, left_throt); 
  }
 }