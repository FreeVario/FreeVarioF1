/*
 FreeVario http://FreeVario.org

  Copyright (c), PrimalCode (http://www.primalcode.org)
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/


#include "buttons.h"
#include "display.h"
#include "boardgpio.h"

uint32_t optbtndelay=0;
uint8_t optionbt = 0;


void BTN_Setup() {

}

#define BUTTONDELAY 3000

/*
 * Function hold button in for a second or so to soft reset mcu.
 * Handy to go back to pre-takeoff condition if there were a false
 * detection.
 * This way the GPS stay locked.
 */
void BTN_Read() {

	if (optionbt) {
		if (HAL_GPIO_ReadPin(FV_BRNPRT, FV_BTNOPTION)) {
			if (HAL_GetTick() > optbtndelay + BUTTONDELAY) {
				togglePowerBankFunction();
				optbtndelay = HAL_GetTick();
			}
		}
	}


}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

	if(GPIO_Pin==FV_BTNOPTION) {
		optbtndelay=HAL_GetTick();
		optionbt=1;
	}

	if(GPIO_Pin==FV_BTNNEXT) {
		DISP_NextMode();
	}

	if(GPIO_Pin==FV_BTNPREV) {
		DISP_PrevMode();
	}

}
