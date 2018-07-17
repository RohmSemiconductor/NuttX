# Overview of this repository
NuttX driver libraries for Rohm Sensors &amp; Bluetooth LE

(NuttX is a real-time operating system (RTOS) with an emphasis on standards compliance and small footprint.
http://nuttx.org/doku.php?id=nuttx)

# Supported devices
* Accelerometer KX122, I2C, SAD=0x1F
* Accelerometer KX224, I2C, SAD=0x1F
* Magnetosensor BM1422AGMV, I2C, SAD=0x0F
* Pressure sensor BM1383AGLV, I2C, SAD=0x5D
* ALS/Proximity sensor RPR-0521RS, I2C, SAD=0x38
* Hall Effect sensor BD7411G, GPIO
* Color sensor BH1749NUC, I2C, SAD=0x38/0x39

 This repository contains the drivers for:
* Accelerometer KX224
* Hall Effect sensor BD7411G
* Color sensor BH1749NUC

This repository contains the <code>makefile</code>, <code>kconfig</code> and <code>cxd56-sensors.c</code> modifications to allow you to easily test these devices.
However these files could be outdated compared to the Sony-provided files, and as such may cause you to lose other functionality.

Should you wish to use these drivers on a newer base, you need to modify:
* <code>sdk/drivers/Kconfig </code>and<code> Make.defs</code>
* <code>sdk/bsp/board/Makefile
* <code>sdk/bsp/board/common/src/cxd56-sensors.c 

The actual driver files are located in:

* <code>sdk/drivers 
* <code>sdk/bsp/board/common
* <code>sdk/bsp/include/nuttx/sensors

The rest of the drivers are available on the [Sony GitHub](https://github.com/sonydevworld/spresense/tree/master/sdk/drivers) , and should come with the SDK.

# How to setup and test devices with Sony Spresense

About Sony Spresense
https://developer.sony.com/develop/spresense/

 For help regarding the installation of the Spresense SDK please refer to
 [Setting up the Spresense SDK](https://developer.sony.com/develop/spresense/developer-tools/get-started-using-nuttx/set-up-the-nuttx-environment "Title")
 


###  Uploading and testing the sensor drivers  ###

1. Run the config and select the drivers and examples you wish to upload 
* In the sdk folder, type <code>tools/config.py --menuconfig</code>
2. Save the configuration and run make
3. Upload the nuttx.spk to the board according to the instructions in the [Flashing tutorial](https://developer.sony.com/develop/spresense/developer-tools/get-started-using-nuttx/set-up-the-nuttx-environment#_flashing_tutorial "Title")
4. When the system has booted, type help. This will show you the built-in applications, which should include the driver example you chose in the config
5. Type the name of the application in the NuttX shell, and the application should begin running