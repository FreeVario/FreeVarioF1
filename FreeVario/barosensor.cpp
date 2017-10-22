/*
 FreeVario http://FreeVario.org
  
  Copyright (c), PrimalCode (http://www.primalcode.org)
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/

 
#include "mbed.h"
#include "config.h"
#include <vector>
#include <numeric>
#include <ms5611.h>                   //http://mbed.org/users/loopsva/code/ms5611/
#include "barosensor.h"
#include "freevario.h"

double realPressureAv = 1; 
double realTemp = 0;
int samplecount;
float currentVarioFts = 0; // ft/s
float cuttentVarioAvFts = 0;
float currentAltitudeFt = 0;

std::vector<float> varioFt(10); //vario ft/s
std::vector<float> altitudeFt(10); // altitude in ft

//adaptive vario
bool vTriggerd = false;
unsigned long vtime = 0;
Timer t;
//


ms5611 baro1(I2C_SDA , I2C_SCL, ms5611::CSBpin_0);  

#if defined(VARIO2)
MS5611 baro2(I2C_SDA , I2C_SCL, ms5611::CSBpin_1);
#endif


void setupBaro(){
  baro1.cmd_reset();
  #if defined(VARIO2)
  baro2.cmd_reset();
  #endif
}

float getAltitudeFt() {
   
   float altitude = baro1.getAltitudeFT(conf.qnePressure);
   
   return altitude;
}

void readTemp() {
  realTemp = baro1.calcTemp(); //calculate press and temp, then returns current temperature in degC
}


//Calculatie the vario and fill the vector arrays
void calcVario() {
  
    currentAltitudeFt = baro1.getAltitudeFT(conf.qnePressure);
    
    altitudeFt.push_back(currentAltitudeFt);
    altitudeFt.erase(altitudeFt.begin());

    
    if (altitudeFt.size() > 9) {
        currentVarioFts =  altitudeFt.at(0) -  altitudeFt.at(9);
    }else{
        currentVarioFts = 0;
    }
    
    cuttentVarioAvFts = accumulate( altitudeFt.begin(), altitudeFt.end(), 0.0)/altitudeFt.size(); 
    
  
  
}

void readBaro() {
samplecount++;
#if defined(VARIO)
  double pressure;
 
  pressure = baro1.getPressure();

#if defined(VARIO2)
  double pressure2;
  double pressure1t;
  double pressure2t;
  pressure2 = baro2.calcPressure();

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


#endif
}
