# Work-in-progress code

This code base is for files that are temporary, for reference, may later be integral to the functionality, or be input / output files to read or write to.

The following files can be used to test the functionality of the BeagleBone Black GPIO ports using a breadboard and LEDs.

inputOutputTest.c is a file to test the input and output functionality of the Beaglebone black GPIO pins. The corresponding header file for this code is gpiofile1.h

blinkingLED.c is a file to test all of the GPIO pins. prugpio.h is the corresponding header file.

Busmonitortest.c is a test file for GPIO pins that are specific to our design. 1575_cape_interface.h is the corresponding header file

cape_test.c is a file to test the the cape with the BBB. The file is meant to be used when the cape is connected to a bus analyzer which is transmitting words. This file is to test the HI-1575 chip functionality, specifically the MR,RVCA,SYNC,REG,and RW pins. cape_test_gpio.h is the corresponding header file.


