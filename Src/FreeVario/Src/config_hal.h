/*
 FreeVario http://FreeVario.org

  Copyright (c), PrimalCode (http://www.primalcode.org)
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>

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

//Audio baseline for buzzer
#define FV_TONEBASE 600

// Master I2C channel
/* Use this for the Baro and ACCL sensor
 * Configure: Fast mode 400
 * Disable the Analog filter if there are a lot of read errors
 * Setting digital filter to 2 (or higher) will help a lot
 */
#define FV_I2C1 hi2c1
#define FV_I2C1_SCL_Pin I2C1_SCL_Pin
#define FV_I2C1_SDA_Pin I2C1_SDA_Pin
#define FV_I2CI_PORT GPIOB


//Secondary I2C channel
/*
 * Todo: also implement Busy flag
 */
#define FV_I2C2 hi2c2


//ACCL address
#define FV_ACCL_ADR SD_MPU6050_Device_0



//GPS
/*
 * Speed: set for correct speed GPS
 * DMA: Enable DMA Channel with circular buffer
 * and set Global interrupt
 * Set polarity low and idle state to set
 */
#define FV_UARTGPS huart2


//BT
/*
 * Set Correct speed
 * DMA not needed
 */
#define FV_UARTBT huart3

//HW timer and output for the piezo buzzer
/*
 * Timer with pwm output
 * Polarity must be set to low
 */
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
#define FV_AMI2C FV_I2C2
#define FV_OLEDI2C FV_I2C2

//Watchdog timer
/*
 * Set prescaler to 32
 */
#define FV_IWDG hiwdg

/*
 * Power management
 *
 */

#define FV_ISCHARGEDPIN GPIO_PIN_8
#define FV_ISCHARGEDPORT GPIOB

#define FV_ISCHARGINGPIN GPIO_PIN_15
#define FV_ISCHARGINGPORT GPIOA

//ADC for reading internal vBat Channel
//ADC1 needs to be enabled for it
//Continuous conversion mode set to enabled
#define FV_HALADC hadc1

/*
 * Buttons
 * Configure all buttons as GPIO_EXTI
 * to use the interrupt handler.
 * Use the same port
 */

#define FV_BTNOPTION GPIO_PIN_3
#define FV_BTNNEXT	GPIO_PIN_5 //B
#define FV_BTNPREV  GPIO_PIN_4 //B
#define FV_BRNPRT 	GPIOB

/*
 * Enable powerbank port
 */

#define FV_PWROUTPIN GPIO_PIN_0
#define FV_PWROUTPORT GPIOB


/*
 * OTG Enable
 */

#define FV_OTGENABLEPIN GPIO_PIN_1
#define FV_OTGENABLEPORT GPIOB

#endif /* CONFIG_HAL_H_ */
