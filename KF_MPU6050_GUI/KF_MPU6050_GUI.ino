/*
Program for reading data from the MPU6050 accelerometer and gyroscope.
    Based on the Korneliusz Jarzebski library:
    MPU6050 Triple Axis Gyroscope & Accelerometer. Pitch & Roll Accelerometer Example.
    GIT: https://github.com/jarzebski/Arduino-MPU6050
*/

#include <Wire.h>
#include <MPU6050.h>
#include "IMU_KalmanFilter.h"


MPU6050 MPU;
KalmanFilter X_Kalman;
KalmanFilter Y_Kalman;
KalmanFilter Z_Kalman;

// variables of data from accelerometer
float normalAccPitch = 0;
float normalAccRoll = 0;
float normalAccYaw = 0;

// variables of data from gyroscope
float normalGyroPitch = 0;
float normalGyroRoll = 0;
float normalGyroYaw = 0;

// values from Kalman Filter
float kalPitch = 0;
float kalRoll = 0;
float kalYaw = 0;

unsigned int startTime = 0;
float timeStep = 0.01;


void setup()
{
  // Initialization connection with imu with 115200 bps
  Serial.begin(115200);
  Serial.println();
  Serial.println("*** Initialization MPU... ***");
  while(!MPU.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G, MPU6050_ADDRESS))
  {
    Serial.println("Not found MPU...");
    delay(1000);
  }
  // calibrate gyro
  MPU.calibrateGyro();
}

void loop()
{
  startTime = millis();
  
  // read normalized values of Acc and Gyro
  Vector normalAcc = MPU.readNormalizeAccel();
  Vector normalGyro = MPU.readNormalizeGyro();
  
  // calculate normalized values of pitch, roll and yaw from acc data
  normalAccPitch = -(atan2(normalAcc.XAxis, sqrt(normalAcc.YAxis * normalAcc.YAxis + normalAcc.ZAxis * normalAcc.ZAxis)) * 180.0) / M_PI;
  normalAccRoll = (atan2(normalAcc.YAxis, normalAcc.ZAxis) * 180.0) / M_PI;
  normalAccYaw = normalAcc.ZAxis;
  
  // calculate values of pitch and roll estimated by Kalman Filter
  kalPitch = Y_Kalman.compute(normalAccPitch, normalGyro.YAxis);
  kalRoll = X_Kalman.compute(normalAccRoll, normalGyro.XAxis);
  kalYaw = Z_Kalman.compute(normalAccYaw, normalGyroYaw);
  
  // calculate normalized values of pitch, roll and yaw from gyro data
  normalGyroPitch += normalGyro.YAxis * timeStep;
  normalGyroRoll += normalGyro.XAxis * timeStep;
  normalGyroYaw += normalGyro.ZAxis * timeStep;
  
  //Serial.print("Normalized obtained values from accelerometer:");
  //Serial.print("\t Normalized Acc Pitch: ");
  Serial.print(normalAccPitch);
  Serial.print("/");
  //Serial.print("\t Normalized Acc Roll: ");
  Serial.print(normalAccRoll);
  Serial.print("/");
  //Serial.print("\t Normalized Acc Yaw: ");
  Serial.print(normalAccYaw);
  Serial.print("/");
  
  //Serial.print("Obtained valus roll and yaw from Kalman Filter:");
  //Serial.print("\t Pitch Kalman value: ");
  Serial.print(kalPitch);
  Serial.print("/");
  //Serial.print("\t Roll Kalman value: ");
  Serial.print(kalRoll);
  Serial.println("/");

  Serial.print(kalYaw);
  Serial.println("/");
  /*
  Serial.print("Normalized obtained values from gyroscope:");
  Serial.print("\t Normalized Gyro Pitch:");
  Serial.print(normalGyroPitch);
  Serial.print("\t Normalized Gyro Roll:");
  Serial.print(normalGyroRoll);
  Serial.print("\t Normalized Yaw: ");
  Serial.println(normalGyroYaw);
  */
  // Frequency of program 1 Hz
  delay(1000);
}
