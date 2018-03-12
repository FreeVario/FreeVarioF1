/*
 FreeVario http://FreeVario.org

  Copyright (c), PrimalCode (http://www.primalcode.org)
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/


#ifndef DISPLAY_H_
#define DISPLAY_H_
#include "config.h"

#define DISPPOWERDATA 0
#define DISPVARIODATA 1

void DISP_Setup();
void DISP_Update();
void DISP_SetMode(uint8_t mode);
static void showVarioData();
static void showPowerData();

#endif /* DISPLAY_H_ */
