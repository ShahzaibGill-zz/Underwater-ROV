#include <MadgwickAHRS.h>
#include "MPU9250.h"
#include <Servo.h>

MPU9250 myIMU;
Servo MOTOR1;
Servo MOTOR2;
Madgwick filter;
unsigned long microsPerReading, microsPrevious;
float accelScale, gyroScale;
boolean is_360_run = false;
float roll, pitch, yaw;
void setup() {
  Serial.begin(9600);
  Wire.begin();
  MOTOR1.attach(12);
  MOTOR2.attach(11);

  // Calibrate gyro and accelerometers, load biases in bias registers
  myIMU.calibrateMPU9250(myIMU.gyroBias, myIMU.accelBias);
  myIMU.initMPU9250();
  // Get magnetometer calibration from AK8963 ROM
  myIMU.initAK8963(myIMU.magCalibration);
  filter.begin(25);

  // initialize variables to pace updates to correct rate
  microsPerReading = 1000000 / 25;
  microsPrevious = micros();
  delay(2000);
}

void loop() {
  measure();
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
    Serial.println(roll);

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

void turn_360() {
  measure();
  float desired_yaw = yaw;
  Serial.print("desired_yaw");
  Serial.print(desired_yaw);
  float current_yaw = yaw;
  while ((current_yaw > (desired_yaw - 10)) && (current_yaw < (desired_yaw + 10)))
  {
    measure();
    current_yaw = yaw;
    Serial.println("Desired_yaw");
    Serial.println(desired_yaw);
    Serial.println("Current yaw");
    Serial.println(current_yaw);
    MOTOR1.write(175);
  }
  while (current_yaw < (desired_yaw - 5) || current_yaw > (desired_yaw + 5))
  {
    Serial.println("Moving till it goes 360");
    measure();
    current_yaw = yaw;
    MOTOR1.write(175);
  }
//Stop the motor
  MOTOR1.write(90);
  Serial.println("Your 360 should be completed");
}


void run_motor(int m1val, int m2val)
{
  MOTOR1.write(m1val);
  MOTOR2.write(m2val);

}
