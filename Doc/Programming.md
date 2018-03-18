Programming the FreeVario V1 board

Requirements:
USB to serial
stm32flash program

Programming:

Connect the usb to serial adapter to Serial 1 on the board, but do not connect the
supply if it is not 3.3V. Use the USB to provide the supply.

Press and hold the next button and then the reset button.
Release the next button

Run: stm32flash -w FreeVario.binary -v -g 0x0 /dev/ttyUSB0 
(filename and USB may vary)

The board will reset 

