# Ethernet-1553
# BBB:
This folder contains all of the files that would go on the Beaglebone Black.

**1575_cape_interface.h** this is the header file for our code. It defines the GPIO ports, as well as some of the memory, and defines some other useful variables.

**1575_functions.c** this file contains all of the functions that are used to interface with the HI-1575 chip. It has functions to read and write the pins, as well as doing some common functionality, such as pulsing some pins. It also enables the GPIO ports as bidirectional and sets them to outputs.

**TCP_Client.c** this file contains all of the functions to use the server.


# CymStar Host:
This folder contains all of the files that would be on the host side.

# Test Files:
This folder contains all of our test files, mainly to test the GPIO functionality.

**inputOutputTest.c** is a file to test the input and output functionality of the Beaglebone black GPIO pins. The corresponding header file for this code is **gpiofile1.h**

**blinkingLED.c** is a file to test all of the GPIO pins. **prugpio.h** is the corresponding header file.

**Busmonitortest.c** is a test file for GPIO pins that are specific to our design.
# OldCode:
This folder contains all of the files from the fall 2022 capstone team. See the ReadMe in this file for more details.
# Getting started:
To start setting up the BeagleBone Black, first connect it to power using the USB cable. Once connected, see the BeagleBone getting started page:
https://www.beagleboard.org/getting-started

Also, the BeagleBone Black system manual:
https://kilobaser.com/wp-content/uploads/2021/02/BBB_SRM.pdf

To connect the BeagleBone Black to the internet, follow these instructions:
https://www.digikey.com/en/maker/blogs/how-to-connect-a-beaglebone-black-to-the-internet-using-usb

A microSD card flashed with a debian image is required to operate the BeagleBone Black before completing step 1. To do this:https://beagleboard.org/Support/BoneScript/updates/

Once the BeagleBone Black is connected to a computer via a USB, the device can be accessed by using IP address 192.168.7.2, which should be the cloud9 IDE. Use the cloud9 terminal to configure the BeagleBone Black with the following steps.

# Step 1: configure /boot/uEnv.txt
To disable audio,HDMI,video,eMMc ports and enable timers

  `sudo nano /boot/uEnv.txt`
  
  Uncomment:
  
  NOTE: do not uncomment disable_uboot_overlay_emmc=1 until you have setup the BBB with the microSD card.
 * disable_uboot_overlay_video=1
 * disable_uboot_overlay_audio=1
 * disable_uboot_overlay_wireless=1
 * disable_uboot_overlay_adc=1
 * disable_uboot_overlay_emmc=1
 
 Add:
 * uboot_overlay_addr0=/lib/firmware/BB-PWM-TIMER-P8.07.dtbo
 * uboot_overlay_addr1=/lib/firmware/BB-PWM-TIMER-P8.08.dtbo
 * uboot_overlay_addr2=/lib/firmware/BB-PWM-TIMER-P8.09.dtbo
 * uboot_overlay_addr3=/lib/firmware/BB-PWM-TIMER-P8.10.dtbo

# Step 2: (for PWM 12.0 MHz Timer Output) In order to easily import/export mappings, add the entries to the rules file

`sudo nano /etc/udev/rules.d/81-pwm-noroot.rules`

* SUBSYSTEM=="pwm", KERNELS=="dmtimer-pwm-4", ENV{PWMCHIP_NAME}="timer4"

* SUBSYSTEM=="pwm", KERNELS=="dmtimer-pwm-5", ENV{PWMCHIP_NAME}="timer5"

* SUBSYSTEM=="pwm", KERNELS=="dmtimer-pwm-6", ENV{PWMCHIP_NAME}="timer6"

* SUBSYSTEM=="pwm", KERNELS=="dmtimer-pwm-7", ENV{PWMCHIP_NAME}="timer7"

then, write in a terminal:

`sudo reboot`

# Step 3: Add all necessary files

On the BeagleBone Black, place the following files in /var/lib/cloud9/BeagleBone/Black/pru

* 1575_cape_interface.h
* 1575_functions.c 
* TCP_Client.c

On the ethernet host, place the following:

# Step 4: Set up Ethernet between host and beaglebone black

# Step 5: Start 12MHz clock, make PRU code run and run client

To use timer7 on the BeagleBone Black

In the terminal, write:

` cd /dev/pwm/timer7`

  `echo 83 > period`
  
  `echo 41 > duty_cycle`
  
  `echo 1 > enable`
  





