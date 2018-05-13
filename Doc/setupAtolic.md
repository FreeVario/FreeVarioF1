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

Open usb_device.c

Add in void MX_USB_DEVICE_Init(void) -> 

  /* USER CODE BEGIN USB_DEVICE_Init_PreTreatment */
          //This will make a device think that the USB device has reconnected
          //Usefull for Android
	  GPIO_InitTypeDef GPIO_InitS;
	  GPIO_InitS.Pin = GPIO_PIN_12;
	  GPIO_InitS.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitS.Pull = GPIO_NOPULL;
	  GPIO_InitS.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOA, &GPIO_InitS);
	  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_RESET);
	  HAL_Delay(300);
  /* USER CODE END USB_DEVICE_Init_PreTreatment */

Configure config_hal.h and config.h if needed
Build project 

Rules:

Only use the FreeVarioLib for development.
Do not include main.c, config_hal.h or config.h or any STM32 hal files

You have to add -u _printf_float to the c linker for Sprintf to work with floats
See: http://www.openstm32.org/forumthread954
