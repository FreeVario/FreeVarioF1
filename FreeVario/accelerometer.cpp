/*
 FreeVario http://FreeVario.org
  
  Copyright (c), PrimalCode (http://www.primalcode.org)
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/


#include "mbed.h"
#include "config.h"
#include "accelerometer.h"
#include "MPU6050.h"


MPU6050 accelgyro(I2C_SDA , I2C_SCL);


float accl[3];
float gyro[3];
int16_t vectoraz;


void setupAccl() {
    
    accelgyro.setAcceleroRange(MPU6050_ACCELERO_RANGE_4G);
    
}

void readAccl() {
    
    float t_accl[3];
    float t_gyro[3];
    accelgyro.getGyro(t_gyro);
    accelgyro.getAccelero(t_accl);
    
    
    accl[0] = (ACCLSMOOTH * accl[0] + t_accl[0]) / (ACCLSMOOTH + 1);
    accl[1] = (ACCLSMOOTH * accl[1] + t_accl[1]) / (ACCLSMOOTH + 1);
    accl[2] = (ACCLSMOOTH * accl[2] + t_accl[2]) / (ACCLSMOOTH + 1);
    vectoraz = (sqrt(pow(accl[0], 2) + pow(accl[1], 2) + pow(accl[2], 2))) - 1;
  
 
    gyro[0] = (ACCLSMOOTH * gyro[0] + t_gyro[0]) / (ACCLSMOOTH + 1);
    gyro[1] = (ACCLSMOOTH * gyro[1] + t_gyro[1]) / (ACCLSMOOTH + 1);
    gyro[2] = (ACCLSMOOTH * gyro[2] + t_gyro[2]) / (ACCLSMOOTH + 1);
    
    
}

