#include <PS2X_lib.h>  //for v1.6

/******************************************************************
 * set pins connected to PS2 controller:
 *   - 1e column: original 
 *   - 2e colmun: Stef?
 * replace pin numbers by the ones you use
 ******************************************************************/
#define PS2_DAT        13  //14    
#define PS2_CMD        11  //15
#define PS2_SEL        10  //16
#define PS2_CLK        12  //17

/******************************************************************
 * select modes of PS2 controller:
 *   - pressures = analog reading of push-butttons 
 *   - rumble    = motor rumbling
 * uncomment 1 of the lines for each mode selection
 ******************************************************************/
//#define pressures   true
#define pressures   false
//#define rumble      true
#define rumble      false


/******************************************************************
 * define control stick calibration values
 ******************************************************************/
#define PSS_LY_FORW_FULL         0
#define PSS_LY_BACK_FULL         255
#define PSS_LY_CENTRE_FORW_LIM   100
#define PSS_LY_CENTRE_BACK_LIM   160

#define PSS_RX_RIGHT_FULL        255
#define PSS_RX_LEFT_FULL         0
#define PSS_RX_CENTRE_RIGHT_LIM  150
#define PSS_RX_CENTRE_LEFT_LIM   110

 /******************************************************************
 * define Motor calibration values
 ******************************************************************/
#define MOTOR_FORW_FULL 180
#define MOTOR_BACK_FULL 0

#define MOTOR_R_STOP_FORW_LIM 95
#define MOTOR_R_STOP_BACK_LIM 75

#define MOTOR_L_STOP_FORW_LIM 95
#define MOTOR_L_STOP_BACK_LIM 75

PS2X ps2x; // create PS2 Controller Class

//right now, the library does NOT support hot pluggable controllers, meaning 
//you must always either restart your Arduino after you connect the controller, 
//or call config_gamepad(pins) again after connecting the controller.

int error = 0;
byte type = 0;
byte vibrate = 0;
int throttle_cmd, l_mtr_cmd, r_mtr_cmd; 

void setup(){
 
  Serial.begin(57600);
  
  delay(300);  //added delay to give wireless ps2 module some time to startup, before configuring it
   
  //CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************
  
  //setup pins and settings: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
  
  if(error == 0){
    Serial.print("Found Controller, configured successful ");
    Serial.print("pressures = ");
  if (pressures)
    Serial.println("true ");
  else
    Serial.println("false");
  Serial.print("rumble = ");
  if (rumble)
    Serial.println("true)");
  else
    Serial.println("false");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Note: Go to www.billporter.info for updates and to report bugs.");
  }  
  else if(error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
   
  else if(error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

  else if(error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
  
//  Serial.print(ps2x.Analog(1), HEX);
  
  type = ps2x.readType(); 
  switch(type) {
    case 0:
      Serial.print("Unknown Controller type found ");
      break;
    case 1:
      Serial.print("DualShock Controller found ");
      break;
    case 2:
      Serial.print("GuitarHero Controller found ");
      break;
  case 3:
      Serial.print("Wireless Sony DualShock Controller found ");
      break;
   }
}

void loop() {
  /* You must Read Gamepad to get new values and set vibration values
     ps2x.read_gamepad(small motor on/off, larger motor strenght from 0-255)
     if you don't enable the rumble, use ps2x.read_gamepad(); with no values
     You should call this at least once a second
   */  
  if(error == 1) //skip loop if no controller found
    return; 

  //if (type == 1) { //DualShock Controller
  ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed
  
  l_mtr_cmd = generateThrottleCmd(PSS_LY_FORW_FULL, PSS_LY_BACK_FULL, PSS_LY_CENTRE_FORW_LIM, PSS_LY_CENTRE_BACK_LIM, 
                                    MOTOR_FORW_FULL, MOTOR_BACK_FULL, MOTOR_L_STOP_FORW_LIM, MOTOR_L_STOP_BACK_LIM);  
  r_mtr_cmd = generateThrottleCmd(PSS_LY_FORW_FULL, PSS_LY_BACK_FULL, PSS_LY_CENTRE_FORW_LIM, PSS_LY_CENTRE_BACK_LIM, 
                                    MOTOR_FORW_FULL, MOTOR_BACK_FULL, MOTOR_R_STOP_FORW_LIM, MOTOR_R_STOP_BACK_LIM);
  
  //generateMotorCmd(l_mtr_cmd, r_mtr_cmd); 
  
  //Serial.println(ps2x.Analog(PSS_RX), DEC); 
  /*
  Serial.print("Left ");
  Serial.print(l_mtr_cmd, DEC);
  Serial.print(" Right ");
  Serial.println(r_mtr_cmd, DEC);
  */
  delay(50);  
}

int generateThrottleCmd(int cntrl_forw_full, int cntrl_back_full, int cntrl_centre_forw_lim, int cntrl_centre_back_lim, 
                        int throttle_forw_full, int throttle_back_full, int throttle_stop_forw_lim, 
                        int throttle_stop_back_lim){
  
  int cntrl_range = cntrl_forw_full - cntrl_back_full;
  int cntrl_dir = cntrl_range / abs(cntrl_range);
  double m, b;

  int throttle_stop = (throttle_stop_forw_lim + throttle_stop_back_lim) / 2 ;
  
  int cntrl_cmd = ps2x.Analog(PSS_LY);
      
  if (cntrl_dir > 0){
    if (cntrl_cmd >= cntrl_centre_back_lim && cntrl_cmd <= cntrl_centre_forw_lim){ 
      return throttle_stop;
    }
    else if (cntrl_cmd > cntrl_centre_forw_lim) {
      m = (throttle_forw_full - throttle_stop_forw_lim) * 1.0 / ( cntrl_forw_full - cntrl_centre_forw_lim);
      b = throttle_forw_full - m * cntrl_forw_full;
    }
    else {
      m = (throttle_back_full - throttle_stop_back_lim) * 1.0 / ( cntrl_back_full - cntrl_centre_back_lim);
      b = throttle_back_full - m * cntrl_back_full;
    }
  }
  else{
    if (cntrl_cmd >= cntrl_centre_forw_lim && cntrl_cmd <= cntrl_centre_back_lim){ 
      return throttle_stop;
    }
    else if (cntrl_cmd < cntrl_centre_forw_lim) {
      m = (throttle_forw_full - throttle_stop_forw_lim) * 1.0 / ( cntrl_forw_full - cntrl_centre_forw_lim);
      b = throttle_forw_full - m * cntrl_forw_full;
    }
    else {
      m = (throttle_back_full - throttle_stop_back_lim) * 1.0 / ( cntrl_back_full - cntrl_centre_back_lim);
      b = throttle_back_full - m * cntrl_back_full;
    }

    return m * cntrl_cmd + b;
  }
}

/*
void generateMotorCmd(int& l_mtr_cmd, int& r_mtr_cmd){
  int cntrl_range = PSS_RX_RIGHT_FULL - PSS_RX_LEFT_FULL;
  int cntrl_dir = cntrl_range / abs(cntrl_range);
  double m, b, scale;

  //int throttle_stop = (throttle_stop_forw_lim + throttle_stop_back_lim) / 2 ;
  
  int cntrl_cmd = ps2x.Analog(PSS_RX);
      
  if (cntrl_dir > 0){
    if (cntrl_cmd >= PSS_RX_CENTRE_LEFT_LIM && cntrl_cmd <= PSS_RX_CENTRE_RIGHT_LIM){ 
      return;
    }
    else if (cntrl_cmd > PSS_RX_CENTRE_RIGHT_LIM) {
      m = 1.0 / ( PSS_RX_RIGHT_FULL - PSS_RX_CENTRE_RIGHT_LIM);
      b = - m * PSS_RX_CENTRE_RIGHT_LIM;
      r_mtr_cmd *= m * cntrl_cmd + b;
    }
    else {
      m = 1.0 / ( PSS_RX_LEFT_FULL - PSS_RX_CENTRE_LEFT_LIM);
      b = - m * PSS_RX_CENTRE_LEFT_LIM;
      l_mtr_cmd *= m * cntrl_cmd + b;
    }
  }
  else{
    if (cntrl_cmd >= PSS_RX_CENTRE_RIGHT_LIM && cntrl_cmd <= PSS_RX_CENTRE_LEFT_LIM){
      return; 
    }
    else if (cntrl_cmd < PSS_RX_CENTRE_RIGHT_LIM) {
      m = 1.0 / ( PSS_RX_RIGHT_FULL - PSS_RX_CENTRE_RIGHT_LIM);
      b = - m * PSS_RX_CENTRE_RIGHT_LIM;
      r_mtr_cmd *= m * cntrl_cmd + b;
    }
    else {
      m = 1.0 / ( PSS_RX_LEFT_FULL - PSS_RX_CENTRE_LEFT_LIM);
      b = - m * PSS_RX_CENTRE_LEFT_LIM;
      l_mtr_cmd *= m * cntrl_cmd + b;
    }
  }  
}
*/
