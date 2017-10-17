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
#include "gpsrunner.h"
#include "freevario.h"
#include "BufferedSerial.h"

//debug section
int d = 0;
//

DigitalOut led1(LED1);
BufferedSerial pc(USBTX, USBRX);

Ticker readSensorTimer;
Ticker sendDataTimer;

std::vector<float> vario(10);
std::vector<float> altitude(10);

uint8_t sensorToken = 0; 


void setupSerial(){
    pc.baud(115200);
}



void sendSensorData() {
    led1 = !led1; 


}



//Fast read sensors one each round
void readSensor() {
    switch(sensorToken) {
        case 0:
            //readbaro
        break;
        case 1:
            //readaccl
        break;
        case 2:
            //reserved
        break;
        
    }
    
    sensorToken++;
    
    if (sensorToken >= 2) {
        sensorToken = 0;
    }
    
}


// main() runs in its own thread in the OS
int main() {
    setupSerial();
    setupGPS();
    readSensorTimer.attach(&readSensor, 0.01);
    sendDataTimer.attach(&sendSensorData,0.1);
    
    while (true) {

      // sleep(); //carefull causes buffer overflow
       
        
    }
}