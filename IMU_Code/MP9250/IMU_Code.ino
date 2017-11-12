#include <MadgwickAHRS.h>
#include "MPU9250.h"
#include <Servo.h>
#include <PID_v1.h>

// IMU VARIABLES
MPU9250 myIMU;
Madgwick filter;
unsigned long microsPerReading, microsPrevious;
float accelScale, gyroScale;
double roll, pitch, yaw;

// MOTOR VARIABLES
Servo MOTOR1;
Servo MOTOR2;

// PID VARIABLES
double Setpoint, Input, Output;
//Define the aggressive and conservative Tuning Parameters
double aggKp=4, aggKi=0, aggKd=1;
double consKp=1, consKi=0, consKd=0.25;
//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint, consKp, consKi, consKd, DIRECT);


void setup() {
  Serial.begin(9600);
  Wire.begin();
//  MOTOR1.attach(12);
//  MOTOR2.attach(11);

//INITIALIZE PID
  Input = 0;
  Setpoint = 0;
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(-180,180);

// Calibrate gyro and accelerometers, load biases in bias registers
  myIMU.calibrateMPU9250(myIMU.gyroBias, myIMU.accelBias);
  myIMU.initMPU9250();
  // Get magnetometer calibration from AK8963 ROM
  myIMU.initAK8963(myIMU.magCalibration);
  filter.begin(25);
  microsPerReading = 1000000 / 25;
  microsPrevious = micros();
  delay(2000);
}

void loop() {
  measure();
  Input = roll;

  double difference = abs(Setpoint-Input);
  if(difference<10)
  {  //we're close to setpoint, use conservative tuning parameters
    myPID.SetTunings(consKp, consKi, consKd);
  }
  else
  {
     //we're far from setpoint, use aggressive tuning parameters
     myPID.SetTunings(aggKp, aggKi, aggKd);
  }
  myPID.Compute();
  Serial.println(Input);
  Serial.println(Output);
  // Set output to Motors.
}

void measure() {
  int aix, aiy, aiz;
  int gix, giy, giz;
  float ax, ay, az;
  float gx, gy, gz;
  unsigned long microsNow;

  // check if it's time to read data and update the filter
  microsNow = micros();
  if (microsNow - microsPrevious >= microsPerReading) {

    // read raw data from IMU
    myIMU.readAccelData(myIMU.accelCount);  // Read the x/y/z adc values
    myIMU.getAres();

    // Now we'll calculate the accleration value into actual g's
    // This depends on scale being set
    myIMU.ax = (float)myIMU.accelCount[0]; // - accelBias[0];
    myIMU.ay = (float)myIMU.accelCount[1]; // - accelBias[1];
    myIMU.az = (float)myIMU.accelCount[2]; // - accelBias[2];

    myIMU.readGyroData(myIMU.gyroCount);  // Read the x/y/z adc values
    myIMU.getGres();

    myIMU.gx = (float)myIMU.gyroCount[0];
    myIMU.gy = (float)myIMU.gyroCount[1];
    myIMU.gz = (float)myIMU.gyroCount[2];

    myIMU.readMagData(myIMU.magCount);  // Read the x/y/z adc values
    myIMU.getMres();
    // User environmental x-axis correction in milliGauss, should be
    // automatically calculated
    myIMU.magbias[0] = +470.;
    // User environmental x-axis correction in milliGauss TODO axis??
    myIMU.magbias[1] = +120.;
    // User environmental x-axis correction in milliGauss
    myIMU.magbias[2] = +125.;

    // The magnometer values  values are not being used right now
    myIMU.mx = (float)myIMU.magCount[0]*myIMU.mRes*myIMU.magCalibration[0] -
               myIMU.magbias[0];
    myIMU.my = (float)myIMU.magCount[1]*myIMU.mRes*myIMU.magCalibration[1] -
               myIMU.magbias[1];
    myIMU.mz = (float)myIMU.magCount[2]*myIMU.mRes*myIMU.magCalibration[2] -
               myIMU.magbias[2];
    myIMU.updateTime();

    ax = convertRawAcceleration(myIMU.ax);
    ay = convertRawAcceleration(myIMU.ay);
    az = convertRawAcceleration(myIMU.az);
    gx = convertRawGyro(myIMU.gx);
    gy = convertRawGyro(myIMU.gy);
    gz = convertRawGyro(myIMU.gz);

    // update the filter, which computes orientation
    filter.updateIMU(gx, gy, gz, ax, ay, az);
    
    roll = filter.getRoll();
    pitch = filter.getPitch();
    yaw = filter.getYaw();
//    Serial.print("Orientation: ");
//    Serial.println(yaw);
//    Serial.print(" ");
//    Serial.println(pitch);
//    Serial.print(" ");
    //Serial.println(roll);
    Input = roll;
    //Serial.println(Input);

    // increment previous time, so we keep proper pace
    microsPrevious = microsPrevious + microsPerReading;
  }
}

float convertRawAcceleration(int aRaw) {
  // since we are using 2G range
  // -2g maps to a raw value of -32768
  // +2g maps to a raw value of 32767
  
  float a = (aRaw * 2.0) / 32768.0;
  return a;
}

float convertRawGyro(int gRaw) {
  // since we are using 250 degrees/seconds range
  // -250 maps to a raw value of -32768
  // +250 maps to a raw value of 32767
  
  float g = (gRaw * 250.0) / 32768.0;
  return g;
}

void run_motors(int m1val, int m2val)
{
  MOTOR1.write(m1val);
  MOTOR2.write(m2val);

}
