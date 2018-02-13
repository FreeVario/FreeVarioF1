/*
 * barosensor.c
 *
 *  Created on: Feb 11, 2018
 *      Author: marco
 */

#include "barosensor.h"
#include "ms5611.h"
#include "stackops.h"
#include <math.h>

int32_t realPressureAv = 1;

extern float currentVarioMPS; // ft/s
extern float cuttentVarioAvMPS;
extern float currentAltitudeMtr;
Queue_t VarioMPS;
Queue_t AltitudeMtr;

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
	SO_setQueue(&VarioMPS, 10);
	SO_setQueue(&AltitudeMtr, 10);
}

float getAltitudeFt() {

	return getAltitudeMt() * 145366.45;

}

double getAltitudeMt() {

	return 44330.0f * (1.0f - pow((double)realPressureAv / (double)conf.qnePressure, 0.1902949f));


}

//This must be called at 100ms
void calcVario() {

    currentAltitudeMtr = getAltitudeMt();
    SO_enqueue(&VarioMPS, currentAltitudeMtr);


    count=0;
    if (SO_qisFull(&VarioMPS)) {
        currentVarioMPS =  SO_front(&VarioMPS) -  SO_rear(&VarioMPS);
        cuttentVarioAvMPS = SO_getAvarage(&VarioMPS);
    }else{
        currentVarioMPS = 0;
    }




}

//This is called at about 20ms
void Baro_GetSensorData() {
	#if defined(VARIO)
	count++;
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


	  realPressureAv = ((conf.variosmooth) * realPressureAv + pressure) / ((conf.variosmooth) + 1);

	#endif


	}
/*
void checkAdaptiveVario(double vario) {
#if defined(ADAPTIVEVARIO)

  double triggerLevel = double(conf.advTriggerLevel)/1000;

  if(takeoff) { //compensate for glider sink
    vario += -double(conf.gliderSinkRate)/1000;
  }


  if (fabs(vario) > triggerLevel && !vTriggerd) { //fabs abs but can handle floats
    t.reset();
    t.start();
    vTriggerd = true;
  }

  int diff = t.read_ms ();

  //Vario level goes back to zero within TriggerTime, increase the filter
  if (vTriggerd && fabs(vario) < triggerLevel  && diff < int(conf.advTriggerTime))  {
    if (conf.variosmooth <= conf.advMaxSmooth ) {
      conf.variosmooth++;
      vTriggerd = false;
      t.stop();
      t.reset();

    }
  }

  //overflow reset
  if (diff > conf.advTriggerTime && vTriggerd) {
    vTriggerd = false;
    t.stop();
    t.reset();
  }
  //Vario stays below trigger level for advRelaxTime, decrease the filter
  if (fabs(vario) < float(triggerLevel) && !vTriggerd && diff >  conf.advRelaxTime) {
    if (conf.variosmooth > conf.advMinSmooth ) {
      conf.variosmooth--;
      t.stop();
      t.reset();
    }
  }
}


*/




