/*
 FreeVario http://FreeVario.org

  Copyright (c), PrimalCode (http://www.primalcode.org)
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/

#include "freevario.h"
#include "audio.h"
#include "barosensor.h"
#include "accelerometer.h"
#include "humidity.h"
#include "senddata.h"
#include <string.h>
#include <stdio.h>

char receiveBuffer[1024];
char *sendBuffer[1024];
uint32_t sc_timer=0;
uint32_t sc_timer100=0;
uint32_t sc_timer1000=0;
uint8_t startwaitcomplete=0;
uint32_t startTime=0;
uint8_t gpsdata=0;

extern TIM_HandleTypeDef FV_10HZTMR;


//GPS data RX interrupt callback
void FV_GpsCallback() {

	gpsdata =1;

}

//check for values that goes out of scale
void watchValues() {
	if (currentVarioMPS >= 99){
		currentVarioMPS = 99;
		BARO_Reset();
	}

	if (currentVarioMPS <= -99) {
		currentVarioMPS = -99;
		BARO_Reset();
	}
}


//Called from main.c
void FV_Run(){
	setup();

	while(1){
		loop();
	}
}

//fast loop
void run10() {
	Baro_Read();
	ACCL_Read();
	if (takeoff) makeVarioAudio((float)currentVarioMPS);
}

//sendata loop
void run100() {

	calcVario();
	if (startwaitcomplete) {
		watchValues();
		checkAdaptiveVario(currentVarioMPS);
		if (gpsdata) {

			strncpy(&sendBuffer, receiveBuffer, sizeof(receiveBuffer));

			CDC_Transmit_FS(&sendBuffer, sizeof(sendBuffer));

			gpsdata = 0;
			HAL_GPIO_TogglePin(FV_LED_GPIO, FV_LED);

		} else {

			sendSensorData();
		}

	}
}

//slow loop
void run1000() {
	HUMID_Read();
#if defined(TESTBUZZER)
			AUDIO_TestToneCall();
#endif


}

static void setup() {
	setupConfig();
	HAL_UART_Receive_DMA(&FV_UART2, receiveBuffer, sizeof(receiveBuffer));
	AUDIO_Setup_Tone();

	BARO_Setup();
	ACCL_Setup();
	HUMID_Setup();

	HAL_Delay(100);

	startTime = HAL_GetTick();

}

static void loop() {

	if ((HAL_GetTick() - startTime) > STARTDELAY) {
		startwaitcomplete = true;
	}

	if (HAL_GetTick() >= (sc_timer + 20)) {
		sc_timer = HAL_GetTick();
		run10();

	}

	if (HAL_GetTick() >= (sc_timer100 + 100)) {
		sc_timer100 = HAL_GetTick();
		run100();

	}

	if (HAL_GetTick() >= (sc_timer1000 + 1000)) {
		sc_timer1000 = HAL_GetTick();
		run1000();

	}

#if defined(TAKEOFFVARIO) && !defined(TESTBUZZER)
	if ((HAL_GetTick() - startTime) > (STARTDELAY + 4000) && !takeoff) {
		if (fabs(currentVarioMPS) > (float) TAKEOFFVARIO) {
			takeoff = true;
		}

	}
#else
	takeoff = true;
#endif

	HAL_Delay(1);

}
