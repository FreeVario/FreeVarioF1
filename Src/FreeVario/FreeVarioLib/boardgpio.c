/*
 FreeVario http://FreeVario.org

  Copyright (c), PrimalCode (http://www.primalcode.org)
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/

/*
 * General internal IO ports
 */

#include "boardgpio.h"

#define CHARGEINTERVALOFF 300
#define CHARGEINTERVALON  60

extern double vbat;
extern uint8_t pbat;
extern uint8_t ischarging;
extern uint8_t ischarged;
uint16_t seconds=0;

extern ADC_HandleTypeDef FV_HALADC;

void BGPIO_Setup() {
	HAL_ADC_Start(&FV_HALADC);

}

//once a second
void BGPIO_Read() {
	readBatVoltage();
	readCharger();
#ifdef INTERVALPWB
	if(ischarging){
		doIntervalCharge();
	}
#endif
}


void doIntervalCharge() {
	if(seconds < CHARGEINTERVALON){
		HAL_GPIO_WritePin(FV_PWROUTPORT,FV_PWROUTPIN,GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(FV_PWROUTPORT,FV_PWROUTPIN,GPIO_PIN_RESET);
	}

	seconds++;

	if (seconds >= CHARGEINTERVALOFF){
		seconds=0;
	}


}

void readBatVoltage() {

	if (HAL_ADC_PollForConversion(&FV_HALADC,100) == HAL_OK) {

			uint32_t cnv = HAL_ADC_GetValue(&FV_HALADC);
			//double vbat = (double)( (cnv * 2 * 3300) / 0xfff)/1000;
			//TODO: figure out the conversion, check for power draining due to measurement
		  vbat = (double) cnv /250;
		  //calculate %charge
		  pbat = (uint8_t)((vbat - 3.6) * 16.6);

	}


}

void readCharger(){

	if(HAL_GPIO_ReadPin(FV_ISCHARGEDPORT,FV_ISCHARGEDPIN) == GPIO_PIN_SET) {
		ischarged = 0;
		 if (HAL_GPIO_ReadPin(FV_ISCHARGINGPORT,FV_ISCHARGINGPIN) == GPIO_PIN_RESET) {
			 ischarging = 1;
		 }

	} else {
		ischarged = 1;
		ischarging = 0;
	}



}

void setPowerBankFunction(uint8_t state){
		HAL_GPIO_WritePin(FV_PWROUTPORT,FV_PWROUTPIN,state);
}

void togglePowerBankFunction() {
	HAL_GPIO_TogglePin(FV_PWROUTPORT,FV_PWROUTPIN);
}


