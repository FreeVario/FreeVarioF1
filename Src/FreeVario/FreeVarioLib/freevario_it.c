/*
 * freevario_it.c
 * STM32 interup call functions.
 * For now manually call functions here from the the interrupt
 * callback source file (eg. stm32f3xx_it.h)
 *  Created on: Feb 8, 2018
 *      Author: marco
 */

#include "freevario_it.h"

extern I2C_HandleTypeDef FV_I2C1;
