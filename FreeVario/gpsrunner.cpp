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
#include "gpsrunner.h"
#include "freevario.h"
#include <vector>
#include <numeric>
#include "BufferedSerial.h"

BufferedSerial SerialGPS(GPSTX, GPSRX); 

#define GPSBUFFER 120
static char GPSbuffer[GPSBUFFER];

volatile bool gps_newline = false;
// Circular buffers for serial TX and RX data - used by interrupt routines
const int buffer_size = 255;
// might need to increase buffer size for high baud rates
char rx_buffer[buffer_size+1];

volatile int rx_in=0;
volatile int rx_out=0;
 char rx_line[80];



// Interupt Routine to read in data from serial port
void Rx_GPS() {
   /*
    while ((SerialGPS.readable()) && (((rx_in + 1) % buffer_size) != rx_out)) {
        rx_buffer[rx_in] = SerialGPS.getc();
        if (rx_buffer[rx_in] == 0x0d) {
          gps_newline = true;
         
        }
        rx_in = (rx_in + 1) % buffer_size;
        
    }
    
    if( gps_newline) {
        sendGPSline();
        gps_newline = false;
    }
    */
   while (SerialGPS.readable()) {
       GPSstuff(SerialGPS.getc());
   }
    return;
}


void GPSstuff(char c) {                                         // GPSbuffer[] is global
  static int i;                                              //   persistent within function scope
  static char q;
  static uint8_t flag = false;
                                    // GPS serial line input buffer
  
  q = c;

  if ( q == 0x24 )                                              // '$'
  {
    i = 0;                                                      // brute force sync on '$' to GPSbuffer[0]
    // Serial << "Found $" << endl;
  }

  if ( i < GPSBUFFER) GPSbuffer[i++] = q;

  if (q == 0x0d) {
    flag = true;                                                // is the character a CR for eol
    GPSbuffer[i++] = '\0';
    i = 0;
  }

  if (flag) {                                                   // test for end of line and if the right GPSbuffer
    flag = false;                                               // reset for next time
    pc.printf("%s\r\n", GPSbuffer);

    memset(GPSbuffer, 0, sizeof(GPSbuffer));
  }
}



void sendGPSline() {
   
    int i;
    i = 0;
   
// Start Critical Section 
   // NVIC_DisableIRQ(GPS_IRQ); //have to test if interupting with readsensors

    while ((i==0) || (GPSbuffer[i-1] != 0x0d)) {

        if (rx_in == rx_out) {
// End Critical Section - need to allow rx interrupt to get new characters for buffer
         //   NVIC_EnableIRQ(GPS_IRQ);
            while (rx_in == rx_out) {
            }
// Start Critical Section - don't interrupt while changing global buffer variables
         //   NVIC_DisableIRQ(GPS_IRQ);
       }
        GPSbuffer[i] = rx_buffer[rx_out];
         
        i++;
        rx_out = (rx_out + 1) % buffer_size;
    }
    pc.printf("%s", GPSbuffer);
 

    
// End Critical Section
 //  NVIC_EnableIRQ(GPS_IRQ);
    
    //memset(rx_line, 0, sizeof(rx_line));
    return;
}

void setupGPS() {
    SerialGPS.baud(SERIALGPSBAUD);
 //   SerialGPS.attach(&Rx_GPS, Serial::RxIrq);
}

bool gpsHasNewLine() {
    if (gps_newline) {
        gps_newline=false;
        
        return true;
    }else{
        return false;
    }
}