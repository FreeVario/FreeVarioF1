Short version:

Use STMCubeMx to create project using ioc file or generate a new one and
use the structure FreeVario/Src/FreeVario and name FreeVario

Open project in Atolic Studo

Use Team to create new Git repo and set the "In repository Folder" to /Src

Do a pull

Rightclick on project and select new-> Source Folder, select FreeVarioLib

Rightclick on added FreeVarioLib folder and select "Add/Remove include Path"

Open main.c
Add->
/* USER CODE BEGIN Includes */
#include "freevario.h"
/* USER CODE END Includes */



And->

  /* USER CODE BEGIN 2 */
  FV_Run();
  /* USER CODE END 2 */

Configure config_hal.h if needed
Build project 

Rules:

Only use the FreeVarioLib for development.
Do not include main.c or config_hal.h 

You have to add -u _printf_float to the c linker for Sprintf to work with floats
See: http://www.openstm32.org/forumthread954
