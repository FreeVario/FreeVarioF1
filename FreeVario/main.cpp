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
#include "freevario.h"
#include "conf.h"
#include <vector>
#include <numeric>
#include "gpsrunner.h"
#include "BufferedSerial.h" //https://os.mbed.com/users/sam_grove/code/BufferedSerial/
#include "barosensor.h"
#include "accelerometer.h"
#include "nmea.h"
#include "humidity.h"
#include "audio.h"

//debug section
//int d = 0;
//

//----------------------------------------------------------------------------//
// Class Loaders
//----------------------------------------------------------------------------//

conf_t conf;


DigitalOut led1(LED1);
Serial pc(USBTX, USBRX);

Timer t_sendata; //100ms
Timer t_readsensor; //10ms x 3
Timer t_slowreadsensor; //1 sec

nmea nmea;


//----------------------------------------------------------------------------//
// Runtime Variables
//----------------------------------------------------------------------------//

uint8_t sensorToken = 0; 



//----------------------------------------------------------------------------//
// Functions
//----------------------------------------------------------------------------//



void setupSerial(){
    pc.baud(115200);
}


//Thread for sending data every 100ms
void sendSensorData() {
    led1 = !led1; 
   // pc.printf("Pressure: %i \r\n", int(realPressureAv * 1000));
    pc.printf("samplecount: %i \r\n",samplecount);
    
    calcVario();
    checkAdaptiveVario(currentVarioFts);
    samplecount = 0; 
       
   if (conf.ptas1) { 
    nmea.setPTAS1(currentVarioFts, cuttentVarioAvFts, currentAltitudeFt); 
    sendSerial(nmea.nmeaPTAS1);
  }      
  
#if defined(ACCL) && defined(DHTH) // kind of a requirement
  
   nmea.setNmeaPcProbeSentence(float((accl[0] * 1000) / 2048)/1000 , float((accl[1] * 1000) / 2048)/1000, float((accl[2] * 1000) / 2048)/1000, dhttemperature * 10, dhthumidity * 10, 0);
   sendSerial(nmea.nmeaPcProbe);
#else
#if defined(ACCL)
   nmea.setNmeaPcProbeSentence(float((accl[0] * 1000) / 2048)/1000 , float((accl[1] * 1000) / 2048)/1000, float((accl[2] * 1000) / 2048)/1000, realTemp, 0, 0);   
   sendSerial(nmea.nmeaPcProbe);
#endif
#endif

}


void sendSerial(char *message){
    pc.printf("%s \r\n", message);
}

void slowReadSensors(){
    
#if defined(DHTH)
    readHumid();
    realTemp = dhttemperature;
     
    
#else
    readTemp(); //expensive process call
#endif
}

//Read sensors one at a time
//Not using interupts as it interferes with i2c callbacks
// 3x 10ms = 30ms per item
void readSensor() {
   sensorToken++; 
   switch(sensorToken) {
      
        case 1:
             readBaro();
        break;
        case 2:
             readAccl();
        break;
        case 3:
         //    makeVarioAudio(currentVarioFts * 0.3048);
        break;
        
    }
    
    
    
    if (sensorToken >= 3) {
        sensorToken = 0;
    }
    
}


// main() runs in its own thread in the OS
int main() {
    setupConfig();
    setupSerial();
    setupGPS();
    setupBaro();
    setupAccl();
     
    t_sendata.start();
    t_readsensor.start();
    t_slowreadsensor.start();
    
    set_time(1256729737); //set the time, otherwise the time function fails
    
    while (true) {
      
       Rx_GPS();
        
        if (t_sendata.read_ms() >= 100) {
            t_sendata.reset();
           
             sendSensorData();
            
            
        }
        
        if (t_readsensor.read_ms() >= 10) {
            t_readsensor.reset();
            
            readSensor();
          
            
        }  
        
         if (t_slowreadsensor.read_ms() >= 2000) {
           t_slowreadsensor.reset();
            
            slowReadSensors();
         
            
        } 
        
        
        
        
       //wait_ms(1);
    }
}