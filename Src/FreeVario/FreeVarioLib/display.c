/*
 FreeVario http://FreeVario.org

  Copyright (c), PrimalCode (http://www.primalcode.org)
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/

#include "display.h"
#include "ssd1306.h"

uint8_t dispmode=0;
extern ADC_HandleTypeDef FV_HALADC;

void DISP_Setup(){
	SSD1306_Init ();  // initialize the display
    SSD1306_Fill (0);  // fill the display with black color
	SSD1306_UpdateScreen(); // update screen

	HAL_ADC_Start(&FV_HALADC);

}

void DISP_Update(){

    SSD1306_Fill (0);  // fill the display with black color
	SSD1306_UpdateScreen(); // update screen

	switch(dispmode) {
	case DISPPOWERDATA:
		showPowerData();
		break;
	case DISPVARIODATA:
		showVarioData();
		break;
	}

}

void DISP_SetMode(uint8_t mode){
	dispmode=mode;
}


void showVarioData(){

		char vals[12];
		SSD1306_GotoXY (10,10);  // goto 10, 10
        sprintf(vals,"%2.2f m/s",currentVarioMPS);
		SSD1306_Puts (vals, &Font_11x18, 1);  // print Hello
		SSD1306_GotoXY (10, 30);
		 sprintf(vals,"%5.2f m",currentAltitudeMtr);
		SSD1306_Puts (vals, &Font_11x18, 1);
		SSD1306_UpdateScreen(); // update screen

}

void showPowerData(){



	    char vals[33];
		SSD1306_GotoXY (10,10);  // goto 10, 10
		if(HAL_GPIO_ReadPin(FV_ISCHARGEDPORT,FV_ISCHARGEDPIN) == 0) {
			SSD1306_Puts ("Charged", &Font_11x18, 1);
		} else if (HAL_GPIO_ReadPin(FV_ISCHARGINGPORT,FV_ISCHARGINGPIN) == 0) {
			SSD1306_Puts ("Charging", &Font_11x18, 1);
		}else {
			SSD1306_Puts ("F-knows", &Font_11x18, 1);
		}

		SSD1306_GotoXY (10, 30);
		if (HAL_ADC_PollForConversion(&FV_HALADC,100) == HAL_OK) {

				uint32_t vbat = HAL_ADC_GetValue(&FV_HALADC);
				sprintf(vals,"%1.2f V",(double)vbat/1000);
				SSD1306_Puts (vals, &Font_11x18, 1);

		}


		SSD1306_UpdateScreen();


}
