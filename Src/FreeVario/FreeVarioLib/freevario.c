/*
 * freevario.c
 * This would be the main file
 *  Created on: Feb 8, 2018
 *      Author: marco
 */

#include "freevario.h"


//Called from main.c
void FV_Run(){
	setup();

	while(1){
		loop();
	}
}

static void setup() {

}


static void loop() {

	HAL_Delay(100);

}
