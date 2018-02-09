/*
 * config_hal.h
 *
 * Processor, channel and pin mappings
 * Only this file should needed to be configured when changing processors
 * Better use FV_ preamble to avoid conflicts
 *  Created on: Feb 8, 2018
 *      Author: marco
 */

#ifndef CONFIG_HAL_H_
#define CONFIG_HAL_H_
#include "stm32f3xx_hal.h"

#define FV_LED LD10_Pin
#define FV_LED_GPIO	GPIOE

#define FV_I2C1 hi2c1


#endif /* CONFIG_HAL_H_ */
