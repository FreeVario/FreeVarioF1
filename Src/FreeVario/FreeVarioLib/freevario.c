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
#include "display.h"
#include "buttons.h"
#include "boardgpio.h"
#include <string.h>
#include <stdio.h>

#define GPSDMABUFFER 128

uint8_t  receiveBuffer[GPSDMABUFFER*2];
uint8_t  transferBuffer[GPSDMABUFFER];

uint32_t sc_timer=0;
uint32_t sc_timer100=0;
uint32_t sc_timer1000=0;
uint32_t sc_timer200=0;
uint8_t startwaitcomplete=0;
uint32_t startTime=0;
volatile uint8_t gpsdata=0;
uint8_t sensorToken = 0;
uint8_t dataValid=1;
uint8_t hasrunonce=0;

#ifdef FV_UARTGPS
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	memcpy(&transferBuffer, &receiveBuffer, sizeof( receiveBuffer));

	gpsdata =1;
}

void HAL_UART_DMARxHalfCplt(UART_HandleTypeDef *UartHandle)
{
	memcpy(&transferBuffer, &receiveBuffer, sizeof( receiveBuffer));

	gpsdata =1;
}
#endif

//Called from main.c
void FV_Run(){
	setup();
	while(1){
		loop();
	}
}

//fast loop
void run10() {
#ifdef FV_BRNPRT
	BTN_Read();
#endif
	sensorToken++;
	switch(sensorToken) {
	case 1:
#ifdef VARIO
		Baro_Read();
#endif
		break;
	case 2:
#ifdef ACCL
		ACCL_Read();
#endif
		break;
	case 3:
#ifdef BUZZER
		if (takeoff) makeVarioAudio((float)currentVarioMPS);
#endif
		break;
	}
	if (sensorToken >= 3) {
		sensorToken = 0;
	}
}


void run100() {

	calcVario();
	if (startwaitcomplete && dataValid) {
		checkAdaptiveVario(currentVarioMPS);

	}

}
//sendata loop
void run200() {
	if (startwaitcomplete && dataValid) {
		sendSensorData();
	}

}

//slow loop
void run1000() {
	if (startwaitcomplete && dataValid) {
#ifdef BGPIO
		BGPIO_Read(); //must happen before display update and per second
#endif
#ifdef FV_OLEDI2C
		DISP_Update();
#endif
	}
#if defined(HUMID)
	HUMID_Read();
#endif
#if defined(TESTBUZZER)
			AUDIO_TestToneCall();
#endif
#ifdef FV_LED
			HAL_GPIO_TogglePin(FV_LED_GPIO, FV_LED);
#endif
}

/*
 *
 * Onetime run after startwait completed
 */

void runOnce(){
#ifdef BUZZERSTARTUPSOUND
	resetTone();
#endif

#ifdef FV_OTGENABLEPIN
	HAL_GPIO_WritePin(FV_OTGENABLEPORT,FV_OTGENABLEPIN,GPIO_PIN_RESET);
#endif

}


void setup() {

#if defined FV_OTGENABLEPIN && defined NOOTGDELAY
	if (HAL_GPIO_ReadPin(FV_BRNPRT, FV_BTNOPTION) == GPIO_PIN_RESET) {
		HAL_GPIO_WritePin(FV_OTGENABLEPORT,FV_OTGENABLEPIN,GPIO_PIN_RESET);

	}
#endif
#ifdef FV_UARTGPS
	HAL_UART_Receive_DMA(&FV_UARTGPS, (uint8_t *)receiveBuffer, GPSDMABUFFER);
#endif
#ifdef BGPIO
	BGPIO_Setup();
#endif
	setupSendData();
#ifdef FV_OLEDI2C
	DISP_Setup();
	DISP_SetMode(DISPPOWERDATA);
	showStartUP();
#endif

	setupConfig();

#ifdef BUZZER
	AUDIO_Setup_Tone();
#ifdef BUZZERSTARTUPSOUND
		StartupTone();
#endif
#endif

#ifdef VARIO
	BARO_Setup();
#endif
#ifdef ACCL
	ACCL_Setup();
#endif
#ifdef HUMID
	HUMID_Setup();
#endif
	HAL_Delay(100);
	startTime = HAL_GetTick();



}

void loop() {

	if(gpsdata) {
		gpsdata=0;

		for (uint8_t i = 0; i < GPSDMABUFFER; ++i) {
				SendDataGPSbuid( &transferBuffer[i]);
		}
	}


	if ((HAL_GetTick() - startTime) > STARTDELAY) {
		startwaitcomplete = true;
		if (!hasrunonce) {
			runOnce();
			hasrunonce=1;
		}

	}

	if (HAL_GetTick() >= (sc_timer + 10)) {
		sc_timer = HAL_GetTick();
		run10();

	}
//Do not change this value, it is used to calculate m/s
	if (HAL_GetTick() >= (sc_timer100 + 100)) {
		sc_timer100 = HAL_GetTick();
		run100();

	}


	if (HAL_GetTick() >= (sc_timer200 + 200)) {
		sc_timer200 = HAL_GetTick();
		run200();

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
#ifdef FV_IWDG
		HAL_IWDG_Refresh ( &FV_IWDG ) ;
#endif


}






