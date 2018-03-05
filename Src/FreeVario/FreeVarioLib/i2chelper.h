/*
 FreeVario http://FreeVario.org

  Copyright (c), PrimalCode (http://www.primalcode.org)
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/



#ifndef I2CHELPER_H_
#define I2CHELPER_H_
#include "config.h"

typedef struct
 {
    I2C_HandleTypeDef* instance;
    uint16_t sdaPin;
    GPIO_TypeDef* i2cPort;
    uint16_t sclPin;
} I2C_Helper_t;

 uint8_t wait_for_gpio_state_timeout(GPIO_TypeDef *port, uint16_t pin, GPIO_PinState state, uint32_t timeout);
void I2C_ClearBusyFlagErratum(I2C_Helper_t* i2c, uint32_t timeout);


#endif /* I2CHELPER_H_ */
