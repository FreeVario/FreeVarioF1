/*
 * barosensor.c
 *
 *  Created on: Feb 11, 2018
 *      Author: marco
 */

#include "barosensor.h"
#include "ms5611.h"

double realPressureAv = 1;

extern float currentVarioFts; // ft/s
extern float cuttentVarioAvFts;
extern float currentAltitudeFt;

stackT varioFt, altitudeFt;

SD_MS5611 baro1;
#if defined(VARIO2)
SD_MS5611 baro2;
#endif




void BARO_Setup(){
	MS5611_Setup( &FV_I2C1, &baro1, MS5611_ADD1);
#if defined(VARIO2)
	MS5611_Setup( &FV_I2C1, &baro2, MS5611_ADD2);
#endif

	StackInit(&varioFt, 10);
	StackInit(&altitudeFt, 10);
}

float getAltitudeFt() {

	return getAltitudeMt() * 145366.45;

}

float getAltitudeMt() {

	return (44330.0f * (1.0f - pow((double)realPressureAv / (double)conf.qnePressure, 0.1902949f)));

}
/*
//This must be called at 100ms
void calcVarioFt() {

    currentAltitudeFt = getAltitudeFt();

    altitudeFt.push_back(currentAltitudeFt);
    altitudeFt.erase(altitudeFt.begin());


    if (altitudeFt.size() > 9) {
        currentVarioFts =  altitudeFt.at(0) -  altitudeFt.at(9);
    }else{
        currentVarioFts = 0;
    }

    cuttentVarioAvFts = accumulate( altitudeFt.begin(), altitudeFt.end(), 0.0)/altitudeFt.size();


}
*/

//This is called at about 20ms
void Baro_GetSensorData() {
	#if defined(VARIO)
	  double pressure;
	  MS5611_readPressure(&FV_I2C1, &baro1);
	  pressure = baro1.uP;

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

	 //TODO: //realPressureAv = (double(conf.variosmooth) * realPressureAv + pressure) / (double(conf.variosmooth) + 1);
	  realPressureAv = (double(conf.variosmooth) * realPressureAv + pressure) / (double(conf.variosmooth) + 1);
	#endif


	}







