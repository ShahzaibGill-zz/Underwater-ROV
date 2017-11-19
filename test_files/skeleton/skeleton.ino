//#include <MadgwickAHRS.h>
//#include "MPU9250.h"
//
//
//enum StatesMachine{
//	CONTROLLER_MODE, // Run Cody's Code
//	IMU_MODE, // Run Shaz's Code
//};
//
//enum StatesMachine roverState; // only  two states for now
//
//unsigned int trianglePressed = 0; // Using a triangle pressed as an exmaple - can be anything
//
//void setup()
//{
//  Serial.begin(9600);
//  Wire.begin();	
//
//  //  Calibration of the IMU
//
//  // Any other setup
//}
//
//
//void loop()
//{
//
//	if (roverState == CONTROLLER_MODE){
//		// Cody put your code here
//
//		if(trianglePressed == 1){
//			roverState = IMU_MODE // Go into IMU mode once triangle(or any button) is pressed
//		}
//	}
//	else if(roverState == IMU_MODE){
//		// Shaz'z code here
//
//		roverState = CONTROLLER_MODE // upon the completion of shaz's code
//	}
//
//	
//}
