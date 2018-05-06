/*
 FreeVario http://FreeVario.org

  Copyright (c), PrimalCode (http://www.primalcode.org)
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>

  Code derived from:
  Temperature and Humidity Sensor mbed library
  by 2014 tomozh <tomozh@gmail.com>

*/


#ifndef AM2322_H_
#define AM2322_H_
#include "../Src/config.h"

typedef struct tagRESULT
{
	float temperature;
	float humidity;
}RESULT;



/** Read current temperature and humidity from AM2321
 * @return  result (true=success)
 */
bool AM2322_poll(I2C_HandleTypeDef *FV_AMI2C);

/** Get last read temperature value
 * @return  temperature value (degress)
 */
float AM2322_getTemperature(void);

/** Get last read humidity value
 * @return  humidity value (%RH)
 */
float AM2322_getHumidity(void);


float getLogicalValue(uint16_t regVal);
uint16_t calcCRC16(const uint8_t* src, int len);


RESULT _result;

#endif /* AM2322_H_ */
