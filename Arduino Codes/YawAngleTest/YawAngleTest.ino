#include <Wire.h>
#include <math.h>
#include "compass.h"

#define PI_APP 3.14159265359
#define dt_s 0.00001
#define RESTRICT_PITCH // Comment out to restrict roll to ±90deg instead 

const int MPU_H = 0x68;  // HANDLE/SEAT  IMU set AD0 to logic high for 0x69
const int MPU_S = 0x69;  // STEER IMU 

/* IMU raw Data */
double accX, accY, accZ;
double gyroX, gyroY, gyroZ;
int16_t tempRaw;
uint32_t timer;

/* Roll and pitch calculated using the accelerometer 
 * while yaw calculated using magnetometer/ accelerometer */
double roll, pitch, yaw; 

//For Seat IMU
double compAngleX, compAngleY, compAngleZ,compAngleZ0;// Calculated angle using a complementary filter
double heading = 0;
bool heading_first_data = true;

////For steer IMU
//double compAngleSteer0; // Base angle
//double compAngleSteer; // Calculated angle using a complementary filter
//int steer_angle_count = 0;  // keeps count 
//
//volatile int averageHandleAngle = 0;
//double PreviousSteer = 0;

/**** Variables for moving average filter ****/
const int numReadings = 10;
int readings[numReadings];      // complementary filter angles
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
double AVG_yaw = 0;


/*********************************************************** IMU functions *******************************************/
void getMPUdata(int address)
{
  Wire.beginTransmission(address);
  Wire.write(0x3B);                         // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(address, 14, true);          // request a total of 14 registers
  accX    = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  accY    = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  accZ    = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  tempRaw = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  gyroX   = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  gyroY   = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  gyroZ   = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

}

void getRPY(int address)
{
  getMPUdata(address);
  
if (address == MPU_S)
  yaw = atan2(-1*(double)accY, (double)accX) * 180 / PI_APP;
  
else
{
#ifdef RESTRICT_PITCH 
  roll  = atan2(accY, accZ) * 180/PI_APP;
  pitch = atan(-accX / sqrt(accY * accY + accZ * accZ)) * 180/PI_APP;

#else 
  roll = atan(accY / sqrt(accX * accX + accZ * accZ)) * 180/PI_APP;
  pitch = atan2(-accX, accZ) * 180/PI_APP;
#endif

} 
}

double MagYaw()
{
  compass_scalled_reading();
  
  double rollAngle  = compAngleX * PI_APP/180;
  double pitchAngle = compAngleY * PI_APP/180;

  double Bfy = compass_z_scalled * sin(rollAngle) - compass_y_scalled * cos(rollAngle);
  double Bfx = compass_x_scalled * cos(pitchAngle) + compass_y_scalled * sin(pitchAngle) * sin(rollAngle) + compass_z_scalled * sin(pitchAngle) * cos(rollAngle);
  double  magYaw = atan2(-Bfy, Bfx) * 180/ PI_APP;

//  Serial.print("magYaw : "); Serial.print(magYaw); Serial.print('\t');
  return magYaw;
  
}

double transformAngle(double baseAngle, double currentAngle)
{
  double angle = (baseAngle - currentAngle)*PI_APP/180;
  angle = atan2(sin(angle),cos(angle))*180/PI_APP;
  
#if 0
  Serial.print(" Angle : "); Serial.print(angle); Serial.print("\t");
  Serial.print(" Base angle : "); Serial.print(baseAngle); Serial.print("\t");
  Serial.print(" Current angle : "); Serial.print(currentAngle); Serial.print("\t");    
#endif

  return angle;
}

/*** Heading IMU update ***/
void updateRollPitchYaw() {

  getRPY(MPU_H);  

  double gyroXrate = gyroX / 131.0; // Convert to deg/s
  double gyroYrate = gyroY / 131.0; // Convert to deg/s
  double gyroZrate = gyroZ / 131.0; // Convter to deg/s

  double dt = (double)(micros() - timer) / 1000000; // Calculate delta time
  timer = micros();
 
/********************  Complementary filter   **********************/
/* compAngel + gyrorate*dt = gyro part */
/* roll, pitch, yaw = accelerometer part */

  compAngleX = 0.85 * (compAngleX + gyroXrate * dt) + 0.15 * roll;
  compAngleY = 0.85 * (compAngleY + gyroYrate * dt) + 0.15 * pitch;
  compAngleZ = 0.85 * (compAngleZ + gyroZrate * dt) + 0.15 * MagYaw(); 

 if (heading_first_data)
  {
    compAngleZ0 = compAngleZ;
    heading_first_data = false;
  }

  heading = transformAngle(compAngleZ, compAngleZ0);
   
  
  #if 1
    Serial.print("Roll : ");Serial.print(compAngleX); Serial.print('\t');
    Serial.print("Pitch : "); Serial.print(compAngleY); Serial.print('\t');
    Serial.print("compAngleZ : "); Serial.print(compAngleZ);Serial.print('\t');
    Serial.print("Heading : "); Serial.println(heading);
  #endif

}

/*********************************************************** END IMU functions ***************************************/

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  
  /*********************** Seat IMU routine ************************/
  /* Rotate the IMU "Takes some time" */ 
  /* Magnetometer default parameters */
//  compass_x_offset = -785.89;
//  compass_y_offset = 1243.25;
//  compass_z_offset = 751.30;
//  compass_x_gainError = 8.39;
//  compass_y_gainError = 8.67;
//  compass_z_gainError = 8.17;

  compass_x_offset = 177.61;
  compass_y_offset = 159.22;
  compass_z_offset = -399.60;
  compass_x_gainError = 8.67;
  compass_y_gainError = 8.88;
  compass_z_gainError = 8.44;
    
  /*** Initialize and Calibrate Magnetometer ***/
  /* compass_offset_calibration(0) 
       Argument 
                " 0 - Use default offset and gain values
                " 1 - Calibrate only for gain values
                " 2 - Calibrate only for offset values
                " 3 - Calibrate for both gain and offset values
  */
  
  compass_init(5);
  compass_debug = 1;
  compass_offset_calibration(0);

  /*** Initialize MPU ***/
  Wire.beginTransmission(MPU_H);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  delay(100);

  /* Get RPY from SEAT IMU */
  getRPY(MPU_H);
  /* set gyro starting angle from accelerometer and magnetometer */
  compAngleX = roll;
  compAngleY = pitch;
  compAngleZ = MagYaw();
  timer = micros();
 /*********************** END Seat IMU routine ************************/ 
 
// /*********************** STEER IMU routine ************************/
//  Wire.beginTransmission(MPU_S);
//  Wire.write(0x6B);  // PWR_MGMT_1 register
//  Wire.write(0);     // set to zero (wakes up the MPU-6050)
//  Wire.endTransmission(true);
//  delay(100);
//
//  /* set gyro starting angle from accelerometer */
//  getRPY(MPU_S);
//  compAngleSteer = yaw;
//
//  /* Initialize the moving average reading */
//  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
//    readings[thisReading] = 0;}
///*********************** END STEER IMU routine ************************/
}


void loop()
{
  updateRollPitchYaw();
  delay(5);
  //updateSteerAngle();
}
