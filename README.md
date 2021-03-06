## Welcome to FreeVario

Freevario is an opensource device to provides telemetry data to any vario based program such as XCSoar or XCTrack via bluetooth or wifi. It is based on de STM32F3 processor. The project includes the board Eagle design and source code.
The current status of the project is beta, it is 100% usable, just some rough edges here and there. Since the paragliding season is active, only bug fixes will be added. 

  <p > 
  <img src="Doc/IMG_20180330_142039618.jpg" width="300"/> <img src="Doc/IMG_20180330_142712066.jpg" width="300"/>  
  </p>
  <p > 
  <img src="Doc/IMG_20180402_114312719.jpg" width="300"/>  <img src="Doc/IMG_20180402_114404188.jpg" width="300"/> 
  </p>

### Features:

Bluetooth, Serial or USB OTG streaming<br>
Solar charger <br>
Power bank (200mA) <br>
Power passthru from external supply <br>
Interval powerbank<br>
Dual pressure sensor support <br>
Adaptive Vario* <br>
Humidity sensor <br>
Accelerometer<br>
Piezo speaker for vario audio <br>
Serial GPS NMEA <br>
NMEA Output PCProbe, PTAS1 and LXNAV<br>

*Adaptive Vario: The sensitivity filter of the barometric sensors are adapted according to the environment. Eg. On a quiet day your flight will be more stable, causing the filter to become more sensitive to changes, this way you will be able to detect the lighter thermals.



#### Bluetooth, Serial or USB OTG streaming

Multiple options available. You can mount a HM-13 bluetooth module to stream BLE or standard bluetooth. Internal serial port can either be used for a i2c OLED display or as an internal serial port for built-in solutions like a Kobo reader.

#### Solar charger

The on board lipo charger can handle multiple power sources including a solar panel to extend the battery life.

#### Powerbank

A on board powerbank can supply up to 200mA 5V via the USB cable to any connected device. When the device is connected to a charger, it will activate a pass through circuit so that the connected device is also charged. This way, disconnecting the usb cable won’t be necessary.
Another option is interval power supply. This will deliver 1 minute out of 5 minutes to the connected device. Most devices won’t support this however. You will need a custom kernel for some Android devices. But is does work with Kobo devices and XCSoar (Top Hat) or LK8000.
