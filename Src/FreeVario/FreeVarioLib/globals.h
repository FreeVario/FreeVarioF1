/*
 * globals.h
 *
 *  Created on: Feb 11, 2018
 *      Author: marco
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

//ALL global references
settings_t conf;
extern I2C_HandleTypeDef FV_I2C1;
extern UART_HandleTypeDef FV_UART2;


float currentVarioMPS; // ft/s
float cuttentVarioAvMPS;
float currentAltitudeMtr;

#endif /* GLOBALS_H_ */
