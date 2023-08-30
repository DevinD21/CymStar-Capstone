# This subfolder is specific to using the File System (Not PRU)
There are many ways to interact with the GPIO pins. These files access the GPIO by going into the sub-folder structure.

This method is slower than the PRU and may not meet the 1553 timing requirements. However, it may provide some insight into how using the system files works with gpio.

Ensure that all the files are in the same folder when running. Running the `BusMonitor.c` should initiate all of the 
