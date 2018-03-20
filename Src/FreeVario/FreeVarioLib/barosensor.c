/*
 FreeVario http://FreeVario.org

  Copyright (c), PrimalCode (http://www.primalcode.org)
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/

#include "barosensor.h"
#include "stackops.h"
#include <math.h>

extern int32_t realPressureAv; // pressure hpa * 100

extern float currentVarioMPS; //
extern float cuttentVarioAvMPS;
extern float currentAltitudeMtr;
extern uint8_t dataValid;
uint8_t takeoff=0;
extern uint8_t startwaitcomplete;
uint8_t vTriggerd;
Queue_t VarioMPS;
Queue_t AltitudeMtr;
uint32_t vtime=0;
int16_t count=0;
uint32_t rtime=0;
uint32_t berrors=0;
int32_t rawPressurePrev = 0; //previous direct reading
int32_t rawPressurePrev2 = 0;

SD_MS5611 baro1;
#if defined(VARIO2)
SD_MS5611 baro2;
#endif




void BARO_Setup(){

	baro1.i2chelper.instance = &FV_I2C1;
	baro1.i2chelper.sclPin = FV_I2C1_SCL_Pin;
	baro1.i2chelper.sdaPin = FV_I2C1_SDA_Pin;
    baro1.i2chelper.i2cPort = FV_I2CI_PORT;
	MS5611_Setup( &baro1, MS5611_ADD1);

#if defined(VARIO2)
	baro2.i2chelper.instance = &FV_I2C1;
	baro2.i2chelper.sclPin = FV_I2C1_SCL_Pin;
	baro2.i2chelper.sdaPin = FV_I2C1_SDA_Pin;
    baro2.i2chelper.i2cPort = FV_I2CI_PORT;
	MS5611_Setup(  &baro2, MS5611_ADD2);
#endif
	SO_setQueue(&VarioMPS, 10); //vario m/s
	SO_setQueue(&AltitudeMtr, 10); //alitude in meters
}

void BARO_Reset(){

	if (HAL_GetTick() - rtime >= 15000) { //prevents reset loop
		MS5611_Reset(&baro1);
#if defined(VARIO2)
		MS5611_Reset(&baro2);
#endif
		BARO_Setup();
		rtime = HAL_GetTick();
	}

}


float getAltitudeFt() {

	return getAltitudeMt() * 145366.45;

}

float getAltitudeMt() {

	return 44330.0f * (1.0f - pow(realPressureAv / (float)conf.qnePressure, 0.1902949f));


}

//This must be called at 100ms
void calcVario() {

    currentAltitudeMtr = getAltitudeMt();

    SO_enqueue(&AltitudeMtr, currentAltitudeMtr);


    count=0;//Debug counter.
    if (SO_qisFull(&AltitudeMtr)) {
        currentVarioMPS =  SO_rear(&AltitudeMtr) -  SO_front(&AltitudeMtr);
        cuttentVarioAvMPS = SO_getAvarage(&VarioMPS);
    }else{
        currentVarioMPS = 0;
    }


}
//Check for erratic values
uint32_t getCleanValues(SD_MS5611  baro) {
	uint32_t pressure = 0;

	if (startwaitcomplete) {
		if (fabs(realPressureAv - baro.uP) < 5000) {
			pressure = baro.uP;
			berrors = 0;
		} else { //invalid data
			I2C_ClearBusyFlagErratum(&baro.i2chelper, 1000);
			MS5611_Reset(&baro);
			berrors++;
			pressure = realPressureAv;
			if (berrors > 100) { //give it up
				pressure = baro.uP;
			}

		}

	} else {

		pressure = baro.uP;
	}

	return pressure;

}

//This is called at about 30ms
void Baro_Read() {

	count++; //Debug counter.
	MS5611_readPressure(&baro1);
	int32_t pressure = getCleanValues(baro1);

#if defined(VARIO2)
	MS5611_readPressure(&baro2);
	int32_t pressure2 = getCleanValues(baro2);
	int32_t pressure1t;
	int32_t pressure2t;

#if defined(VARIO2LEASTDEV)
	int32_t diff = rawPressurePrev - pressure;
	int32_t diff2 = rawPressurePrev2 - pressure2;

	//alter the sensor reading
	//This method is to filter electrical noise
	if (fabs(diff) > fabs(diff2)) { //if the primary has more deviation use the deviation of the secondary sensor
		pressure1t = rawPressurePrev + diff2;
		pressure2t = pressure2;

	} else {
		pressure2t = rawPressurePrev2 + diff;
		pressure1t = pressure;
	}
	rawPressurePrev = pressure;
	rawPressurePrev2 = pressure2;
	pressure = (pressure1t + pressure2t) / 2;

#else

	pressure = (pressure + pressure2) / 2;

#endif
#endif

	  realPressureAv = (conf.variosmooth * realPressureAv + pressure) / (conf.variosmooth + 1);

	}

void checkAdaptiveVario(float vario) {
#if defined(ADAPTIVEVARIO)

  double triggerLevel = (double)(conf.advTriggerLevel)/1000;

  if(takeoff) { //compensate for glider sink
    vario += -(double)(conf.gliderSinkRate)/1000;
  }


  if (fabs(vario) > triggerLevel && !vTriggerd) { //fabs abs but can handle floats
	vtime = HAL_GetTick();
    vTriggerd = true;
  }

  int diff = HAL_GetTick() - vtime;

  //Vario level goes back to zero within TriggerTime, increase the filter
  if (vTriggerd && fabs(vario) < triggerLevel  && diff < (int)(conf.advTriggerTime))  {
    if (conf.variosmooth <= conf.advMaxSmooth ) {
      conf.variosmooth++;
      vTriggerd = false;
      vtime = HAL_GetTick();

    }
  }

  //overflow reset
  if (diff > conf.advTriggerTime && vTriggerd) {
    vTriggerd = false;
    vtime = HAL_GetTick();;
  }
  //Vario stays below trigger level for advRelaxTime, decrease the filter
  if (fabs(vario) < (float)(triggerLevel) && !vTriggerd && diff >  conf.advRelaxTime) {
    if (conf.variosmooth > conf.advMinSmooth ) {
      conf.variosmooth--;
      vtime = HAL_GetTick();
    }
  }
#endif
}







