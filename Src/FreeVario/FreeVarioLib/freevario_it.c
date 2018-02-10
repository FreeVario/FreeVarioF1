/*
 FreeVario http://FreeVario.org

  Copyright (c), PrimalCode (http://www.primalcode.org)
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/



#include "freevario_it.h"
#include "audio.h"

//extern I2C_HandleTypeDef FV_I2C1;
extern uint8_t receiveBuffer[120];

uint8_t sendBuffer[120];


//GPS data RX interrupt callback
void FV_GpsCallback() {


	  memcpy(sendBuffer,receiveBuffer,sizeof(receiveBuffer));
	  CDC_Transmit_FS(sendBuffer, sizeof(sendBuffer));
	  HAL_GPIO_TogglePin(FV_LED_GPIO, FV_LED);


}

//pass through to audio.h
void FV_TonePeriodCall() {

	AUDIO_TimerCall();

}
