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

#define FV_LED LD9_Pin
#define FV_LED_GPIO	GPIOE

#define FV_I2C1 hi2c1
#define FV_UART2 huart2

#define FV_TONETMR htim1
#define FV_TONECHN TIM_CHANNEL_1
#define FV_TONECCR CCR1
#define FV_TONEHALTMR TIM1
#define FV_TONEPERIODTMR htim2
#define FV_TONEPERIODHALTMR TIM2



#endif /* CONFIG_HAL_H_ */
