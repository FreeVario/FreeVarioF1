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

//Board LED
#define FV_LED LD9_Pin
#define FV_LED_GPIO	GPIOE

//I2C channel
#define FV_I2C1 hi2c1

//GPS
#define FV_UARTGPS huart2


//BT
#define FV_UARTBT huart3

//HW timer and output for the piez o buzzer
#define FV_TONETMR htim1
#define FV_TONECHN TIM_CHANNEL_1
#define FV_TONECCR CCR1
#define FV_TONEHALTMR TIM1

//HW DAC output for the speaker
#define FV_DACTMR htim6
#define FV_DACHALTMR TIM6
#define FV_DAC hdac
#define FV_DAC_CHANNEL DAC_CHANNEL_1


//Old gpio input format for DHT sensors, not implemented yet (might never be)
//#define FV_DHTPIN GPIO_PIN_1
//#define FV_DHTPORT GPIOB

//New AM232x I2C based humidity sensors I2C port
//This will use the serial 1 connection on the FreeVario board.
//Serial won't be available for serial data
#define FV_AMI2C hi2c1

#endif /* CONFIG_HAL_H_ */
