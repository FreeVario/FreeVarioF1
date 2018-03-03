/*
 FreeVario http://FreeVario.org

  Copyright (c), PrimalCode (http://www.primalcode.org)
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/

#include "barosensor.h"
#include "ms5611.h"
#include "stackops.h"
#include <math.h>

extern int32_t realPressureAv; // pressure hpa * 100

extern float currentVarioMPS; //
extern float cuttentVarioAvMPS;
extern float currentAltitudeMtr;
uint8_t takeoff=0;
uint8_t vTriggerd;
Queue_t VarioMPS;
Queue_t AltitudeMtr;
uint32_t vtime=0;
int16_t count=0;

SD_MS5611 baro1;
#if defined(VARIO2)
SD_MS5611 baro2;
#endif




void BARO_Setup(){
	MS5611_Setup( &FV_I2C1, &baro1, MS5611_ADD1);
#if defined(VARIO2)
	MS5611_Setup( &FV_I2C1, &baro2, MS5611_ADD2);
#endif
	SO_setQueue(&VarioMPS, 10); //vario m/s
	SO_setQueue(&AltitudeMtr, 10); //alitude in meters
}

void BARO_Reset(){
	MS5611_Reset( &FV_I2C1, &baro1);
#if defined(VARIO2)
	MS5611_Reset( &FV_I2C1, &baro2);
#endif

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
        currentVarioMPS =  SO_front(&AltitudeMtr) -  SO_rear(&AltitudeMtr);
        cuttentVarioAvMPS = SO_getAvarage(&VarioMPS);
    }else{
        currentVarioMPS = 0;
    }




}

//This is called at about 20ms
void Baro_Read() {

	count++; //Debug counter.
	  MS5611_readPressure(&FV_I2C1, &baro1);
	  int32_t pressure = baro1.uP;

	#if defined(VARIO2)
	  double pressure2;
	  double pressure1t;
	  double pressure2t;

	  MS5611_readPressure(&FV_I2C1, &baro2);
	  pressure2 = baro2.uP;

	#if defined(VARIO2LEASTDEV)
	  double diff = rawPressurePrev - pressure;
	  double diff2 = rawPressurePrev2 - pressure2;

	  //alter the sensor reading
	  if (fabs(diff) > fabs(diff2)) { //if the primary has more deviation use the deviation of the secondary sensor
	    pressure1t =  rawPressurePrev + diff2;
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







