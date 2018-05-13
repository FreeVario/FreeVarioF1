/*
 FreeVario http://FreeVario.org

  Copyright (c), PrimalCode (http://www.primalcode.org)
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/
//The accelerometer is purely used for G forces


#include "accelerometer.h"
#include "sd_hal_mpu6050.h"
#ifdef ACCL
SD_MPU6050 mpu1;

extern float accel_x;
extern float accel_y;
extern float accel_z;

extern float gyro_x;
extern float gyro_y;
extern float gyro_z;



void ACCL_Setup() {
	 SD_MPU6050_Init(&FV_I2C1,&mpu1,FV_ACCL_ADR,SD_MPU6050_Accelerometer_16G,SD_MPU6050_Gyroscope_250s );
}


void ACCL_Read() {

	SD_MPU6050_ReadAll(&FV_I2C1,&mpu1);
	//we use the full scale so the devider is 2048

	accel_x = (float) (ACCLSMOOTH * accel_x +  mpu1.Accelerometer_X/2048) / (ACCLSMOOTH + 1);
	accel_y = (float) (ACCLSMOOTH * accel_y +  mpu1.Accelerometer_Y/2048) / (ACCLSMOOTH + 1);
	accel_z = (float) (ACCLSMOOTH * accel_z +  mpu1.Accelerometer_Z/2048) / (ACCLSMOOTH + 1);

	gyro_x = (ACCLSMOOTH * gyro_x +  mpu1.Gyroscope_X) / (ACCLSMOOTH + 1);
    gyro_y = (ACCLSMOOTH * gyro_y +  mpu1.Gyroscope_Y) / (ACCLSMOOTH + 1);
	gyro_z = (ACCLSMOOTH * gyro_z +  mpu1.Gyroscope_Z) / (ACCLSMOOTH + 1);


}

#endif
