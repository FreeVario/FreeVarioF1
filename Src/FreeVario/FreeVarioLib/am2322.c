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





#include "am2322.h"

const int AM2321_I2C_ADDR = 0xB8;



float getLogicalValue(uint16_t regVal)
{
    if(regVal & 0x8000)
    {
        regVal &= ~0x8000;
        return (float)regVal / -10.0;
    }
    else
    {
        return (float)regVal / 10.0;
    }
}

uint16_t calcCRC16(const uint8_t* src, int len)
{
    uint16_t crc = 0xFFFF;

    while(len--)
    {
        crc ^= *(src++);

        for(uint8_t i = 0; i < 8; i++)
        {
            if(crc & 0x01)
            {
                crc >>= 1;
                crc ^= 0xA001;
            }
            else
            {
                crc >>= 1;
            }
        }
    }

    return crc;
}

bool AM2322_poll(I2C_HandleTypeDef *FV_AMI2C)
{
    bool    success = false;
    uint8_t data[8];

    const static uint8_t READ_REGISTER_CMD[] = {
           0x03     // Read register command
         , 0x00     // start addrress
         , 0x04     // read length
          };

    // wakeup
    HAL_I2C_Master_Transmit(FV_AMI2C,AM2321_I2C_ADDR,NULL, 0,100);


    // read data


    HAL_I2C_Master_Transmit(FV_AMI2C,AM2321_I2C_ADDR,(uint8_t *)READ_REGISTER_CMD, 3,100);
    HAL_Delay(2);


    if(HAL_I2C_Master_Receive(FV_AMI2C,AM2321_I2C_ADDR,(uint8_t *)data,8,100) == 0)
    {
        uint8_t  cmd     = data[0];
        uint8_t  dataLen = data[1];
        uint16_t humiVal = (data[2] * 256) + data[3];
        uint16_t tempVal = (data[4] * 256) + data[5];
        uint16_t recvCRC = data[6] + (data[7] * 256);
        uint16_t chkCRC  = calcCRC16(&data[0], 6);

        if(dataLen == 4)
        {
            if(recvCRC == chkCRC)
            {
                if(cmd == 0x03)
                {
                    _result.temperature = getLogicalValue(tempVal);
                    _result.humidity    = getLogicalValue(humiVal);

                    success = true;
                }
            }
        }
    }

    return success;
}

float AM2322_getTemperature(void)
{
    return _result.temperature;
}

float AM2322_getHumidity(void)
{
    return _result.humidity;
}
