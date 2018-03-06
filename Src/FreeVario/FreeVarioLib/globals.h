/*
 FreeVario http://FreeVario.org

  Copyright (c), PrimalCode (http://www.primalcode.org)
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/

#ifndef GLOBALS_H_
#define GLOBALS_H_



//ALL global references
settings_t conf;
extern I2C_HandleTypeDef FV_I2C1;
#ifdef FV_I2C2
extern I2C_HandleTypeDef FV_I2C2;
#endif
extern UART_HandleTypeDef FV_UARTGPS;
extern UART_HandleTypeDef FV_UARTBT;
extern IWDG_HandleTypeDef FV_IWDG;

//DHT TIMER
extern TIM_HandleTypeDef FV_DHTTIMER;

float currentVarioMPS; // ft/s
float cuttentVarioAvMPS;
float currentAltitudeMtr;
int32_t realPressureAv;  // pressure hpa * 100

float accel_x;
float accel_y;
float accel_z;
float gyro_x;
float gyro_y;
float gyro_z;

float humidity;
float humidtemp;



extern uint8_t takeoff;

#endif /* GLOBALS_H_ */
