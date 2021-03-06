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

int8_t dispmode=0;
uint8_t refreshcount=0;
uint32_t sleeptime=0;
extern double vbat;
extern uint8_t ischarging;
extern uint8_t ischarged;
uint8_t isDisplayOn=1;


void DISP_Setup(){
	SSD1306_Init ();  // initialize the display
    SSD1306_Fill (0);  // fill the display with black color
	SSD1306_UpdateScreen(); // update screen

}

void DISP_Refresh() {

	SSD1306_Fill(0);  //not needed if only characters replaced
	UpdateScreen();
	refreshcount = 0;

}

void UpdateScreen() { //extra layer for power saving
	if (isDisplayOn) {
		SSD1306_UpdateScreen();
	}


}

void resetSleepTimer(){ //called by buttons.c
	sleeptime = HAL_GetTick();
	SSD1306_ON();
	isDisplayOn=1;
}

void DISP_Update(){
	refreshcount++;
	if (refreshcount > 10) {
		DISP_Refresh();
	}

	switch(dispmode) {
	case DISPPOWERDATA:
		showPowerData();
		break;
	case DISPVARIODATA:
		showVarioData();
		break;
	case DISPWEATHERDATA:
		showWeatherData();
		break;
	}

#ifdef DISPLAYSLEEP
	if (HAL_GetTick() - sleeptime > DISPLAYSLEEP * 1000) {
		SSD1306_OFF();
		isDisplayOn=0;
	}

#endif

}

void DISP_SetMode(uint8_t mode){
	dispmode=mode;
}

void DISP_NextMode(){
	dispmode++;
	if(dispmode > DISPITEMS){
		dispmode=0;
	}
	DISP_Update();
	DISP_Refresh();
}

void DISP_PrevMode(){
	dispmode--;
	if(dispmode < 0){
		dispmode = DISPITEMS;
	}
	DISP_Update();
	DISP_Refresh();
}

void showVarioData(){

		char vals[12];
		SSD1306_GotoXY (10,10);  // goto 10, 10
        sprintf(vals,"%+1.2f m/s",currentVarioMPS);
		SSD1306_Puts (vals, &Font_11x18, 1);  // print Hello
		SSD1306_GotoXY (10, 30);
		 sprintf(vals,"%05.2f m",currentAltitudeMtr);
		SSD1306_Puts (vals, &Font_11x18, 1);
		UpdateScreen(); // update screen

}

void showStartUP() {
	SSD1306_Fill(0);
	UpdateScreen();

	SSD1306_GotoXY (10,10);  // goto 10, 10

	SSD1306_Puts ("Starting..", &Font_11x18, 1);  // print Hello
	UpdateScreen(); // update screen
}

void showWeatherData(){
	char vals[12];
	SSD1306_GotoXY (10,10);  // goto 10, 10
    sprintf(vals,"%0d hp",(int)realPressureAv/100);
	SSD1306_Puts (vals, &Font_11x18, 1);  // print Hello
	SSD1306_GotoXY (10, 30);
	 sprintf(vals,"%02.1f%% %02.fC",humidity,humidtemp);
	SSD1306_Puts (vals, &Font_11x18, 1);
	UpdateScreen(); // update screen


}

void showPowerData(){


#ifdef FV_PWROUTPORT
	char vals[33];
	SSD1306_GotoXY(10, 10);  // goto 10, 10
	if (HAL_GPIO_ReadPin(FV_PWROUTPORT, FV_PWROUTPIN) == GPIO_PIN_SET) {
		SSD1306_Puts("Power Bank", &Font_11x18, 1);
	} else {
		if (ischarged) {
			SSD1306_Puts("Charged", &Font_11x18, 1);
		} else if (ischarging) {
			SSD1306_Puts("Charging", &Font_11x18, 1);
		} else {
			SSD1306_Puts("Ready", &Font_11x18, 1);
		}
	}


	SSD1306_GotoXY(10, 30);

	sprintf(vals, "%2.2fV %d%%", vbat,pbat);
	SSD1306_Puts(vals, &Font_11x18, 1);

	UpdateScreen();
#endif

}
