#include <PS2X_lib.h>  //for v1.6

/******************************************************************
 * set pins connected to PS2 controller:
 *   - 1e column: original 
 *   - 2e colmun: Stef?
 * replace pin numbers by the ones you use
 ******************************************************************/
#define PS2_DAT        13  //14  brown from controller - pink/red inbetween/ - red to pull up - green on arduino   
#define PS2_CMD        11  //15  orange on controller - brown on teather - orange on arduino
#define PS2_SEL        10  //16  yellow - pink and yellow -yellow on arduino
#define PS2_CLK        12  //17  pruple PS2 - white - orange/gold - brown

//GND black on controller - green - green
//PWR - red-black-green/white-white 

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

 /******************************************************************
 * define Motor command values
 ******************************************************************/
#define MAX_THROTTLE 127


PS2X ps2x; // create PS2 Controller Class

//right now, the library does NOT support hot pluggable controllers, meaning 
//you must always either restart your Arduino after you connect the controller, 
//or call config_gamepad(pins) again after connecting the controller.

int error = 0;
byte type = 0;
byte vibrate = 0;


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
  int l_mtr_cmd, r_mtr_cmd; 
  ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed

  generateMotorCmd(l_mtr_cmd, r_mtr_cmd); 
  
  //Serial.println(l_mtr_cmd, DEC); 
  
  Serial.print("Left ");
  Serial.print(l_mtr_cmd, DEC);
  Serial.print(" Right ");
  Serial.println(r_mtr_cmd, DEC);
  
  delay(50);  
}

void generateMotorCmd(int& l_mtr_cmd, int& r_mtr_cmd){
  int throttle, l_throttle, r_throttle;

  throttle = generateThrottleCmd();
  scaleThrottleForSteer(l_throttle, r_throttle, throttle); 
  throttle2MotorCmd(l_throttle, r_throttle, l_mtr_cmd, r_mtr_cmd);
}


int generateThrottleCmd(){
  int pss_ly = ps2x.Analog(PSS_LY);
  double m;
  int b; 

  // Forward Motion
  if(pss_ly > PSS_LY_CENTRE_FORW_LIM && pss_ly <= PSS_LY_FORW_FULL || 
     pss_ly >= PSS_LY_FORW_FULL && pss_ly < PSS_LY_CENTRE_FORW_LIM){
       
    m = 1.0 * MAX_THROTTLE / (PSS_LY_FORW_FULL - PSS_LY_CENTRE_FORW_LIM);
    b = -m * PSS_LY_CENTRE_FORW_LIM;
  }

  // Reverse Motion
  else if(pss_ly >= PSS_LY_BACK_FULL && pss_ly < PSS_LY_CENTRE_BACK_LIM ||
          pss_ly > PSS_LY_CENTRE_BACK_LIM && pss_ly <= PSS_LY_BACK_FULL){ 
    m = -1.0 * MAX_THROTTLE / (PSS_LY_BACK_FULL - PSS_LY_CENTRE_BACK_LIM);
    b = -m * PSS_LY_CENTRE_BACK_LIM;
  }

  // Standstill
  else{
   m = 0;
   b = 0;
  }
  return m * pss_ly + b;
}


void scaleThrottleForSteer(int& l_throttle, int& r_throttle, int throttle){
  int pss_rx = ps2x.Analog(PSS_RX);
  double r_scale = 1.0, l_scale = 1.0;
  
  // Turn Right
  if(pss_rx > PSS_RX_CENTRE_RIGHT_LIM && pss_rx <= PSS_RX_RIGHT_FULL || 
     pss_rx >= PSS_RX_RIGHT_FULL && pss_rx < PSS_RX_CENTRE_RIGHT_LIM){ 
     r_scale = 1.0 * (pss_rx - PSS_RX_RIGHT_FULL) / (PSS_RX_CENTRE_RIGHT_LIM - PSS_RX_RIGHT_FULL);
  }

  // Turn Left
  else if(pss_rx >= PSS_RX_LEFT_FULL && pss_rx < PSS_RX_CENTRE_LEFT_LIM || 
          pss_rx > PSS_RX_CENTRE_LEFT_LIM && pss_rx <= PSS_RX_LEFT_FULL){ 
     l_scale = 1.0 * (pss_rx - PSS_RX_LEFT_FULL) / (PSS_RX_CENTRE_LEFT_LIM - PSS_RX_LEFT_FULL);
  }

  l_throttle = l_scale * throttle;
  r_throttle = r_scale * throttle;
  return;
}

void throttle2MotorCmd(int l_throttle, int r_throttle, int& l_mtr_cmd, int& r_mtr_cmd){
  double m_l, m_r;
  int b_l, b_r;

  //--- Left Motor ---//

  if (l_throttle == 0){
    l_mtr_cmd = (MOTOR_L_STOP_FORW_LIM + MOTOR_L_STOP_BACK_LIM) / 2;
  }
  else{
    // Forward Motion
    if (l_throttle > 0){
      m_l = 1.0 * (MOTOR_FORW_FULL - MOTOR_L_STOP_FORW_LIM) / MAX_THROTTLE;
      b_l = MOTOR_L_STOP_FORW_LIM;
    }
  
    // Backward Motion
    else if (l_throttle < 0){
      m_l = -1.0 * (MOTOR_BACK_FULL - MOTOR_L_STOP_BACK_LIM) / MAX_THROTTLE;
      b_l = MOTOR_L_STOP_BACK_LIM;
    }

    l_mtr_cmd = m_l * l_throttle + b_l;

  }
  //--- Right Motor ---//
  if (r_throttle == 0){
    r_mtr_cmd = (MOTOR_R_STOP_FORW_LIM + MOTOR_R_STOP_BACK_LIM) / 2;
  }
  else{
    // Forward Motion
    if (r_throttle > 0){
      m_r = 1.0 * (MOTOR_FORW_FULL - MOTOR_R_STOP_FORW_LIM) / MAX_THROTTLE;
      b_r = MOTOR_R_STOP_FORW_LIM;
    }

    // Backward Motion
    else if (r_throttle < 0){
      m_r = -1.0 * (MOTOR_BACK_FULL - MOTOR_R_STOP_BACK_LIM) / MAX_THROTTLE;
      b_r = MOTOR_R_STOP_BACK_LIM;
    }

     r_mtr_cmd = m_r * r_throttle + b_r;
  }    
}
