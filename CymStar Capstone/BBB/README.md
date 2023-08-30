# Code that will be uploaded on the Beaglebone Black
1575_cape_interface.h: this is the header file for our code. It defines the GPIO ports, as well as some of the memory, and defines some other useful variables.

Useful GPIO references:
https://kilobaser.com/wp-content/uploads/2021/02/BBB_SRM.pdf

https://kilobaser.com/beaglebone-black-gpios/

https://vadl.github.io/beagleboneblack/2016/07/29/setting-up-bbb-gpio

1575_functions.c: this file contains all of the functions that are used to interface with the HI-1575 chip. It has functions to read and write the pins, as well as doing some common functionality, such as pulsing some pins. It also enables the GPIO ports as bidirectional and sets them to outputs.

TCP_Client.c: this file contains all of the functions to use the server.


When prompted to enter a username or password, enter the following...
username: `debian`
password: `temppwd`

Turning off the Beaglebone Black by unplugging it can lead to damage. Instead, use the command: `sudo shutdown -h now`

To check the current state of all the GPIO's : `cat /sys/kernel/debug/gpio`
