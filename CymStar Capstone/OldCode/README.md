

# BBB-IO
This folder contains all files needed to set up BeagleBone Black I/O with the on-board Programmable Real-Time Unit (PRU) and the TCP ethernet configuaration and connection.

The PRU is a 200 MHz microcontroller that gives direct and fast access to the I/O pins on the BeagleBone Black. The PRU on-board the BeagleBone Black is used to control the gpio outputs for the HI-1575 chip through communication with the py-uio client io-test.py.

# BBB-IOTEST
This folder contains a short test for an Mbed LPC1768 to test the GPIO pins for functionality

# VM
This folder contains all the files meant to run on the CymHOST Virtual Machine. This include the TCPServer.c, TCPServer.py, and the remote_terminal_initialization.c


* <b>IORun.pru0.c</b> is a file to be compiled into the firmware for the PRU. Once compiled, the machine code runs directly on the PRU.
* <b>io-run.py</b> is the client that will read the shared memory between the AM3358 Processor and the PRU. 
The io-run.py client uses the python module py-uio. py-uio is a python library for Userspace I/O developed by Matthijs van Duin, https://github.com/mvduin/py-uio. This file also implements the TCP client connection for the BeagleBone Black.
* <b>prugpio.h</b> is a header file containing definitions of necessary gpio addresses, bit masks, and methods for writing and reading gpio pins to interface with the HI-1575 MIL-STD-1553 Transceiver and Encoder/Decoder.
* <b>TCPServer.py</b> is a simple TCPServer based on the instructions of Networking from Cymstar


Extra files : 
* <b>TCPClient.c</b> is a C implementation of the client-side BeagleBone Black ethernet transfer.
* <b>TCPServer.c and RunTCPServer.c</b> is a C implementation of the server-side CymHost ethernet transfer 
* <b>bbb-iotest/main.cpp</b> is an Mbed file for the LPC1768 microcontroller that was used to test the GPIO inputs and outputs
* <b>IOTest.pru0.c</b> is a file that contains test methods for GPIO pins for the PRU.
* <b>TCPClient.py</b> is a simple TCPClient based on the instructions of Networking from Cymstar
* <b> BeagleBone Black PRU Process Diagram </b> is an overview of how the Remote Terminal processing functionality is implemented in the software 






To get started setting up the BeagleBone Black, first connect your BeagleBone Black and power it on. For more specific instructions, see the BeagleBoard Getting Started Page: 

Once the BeagleBone Black is connected via USB to a computer, the device will run as a server, communicating with your machine on the IP address 192.168.7.1 or 192.168.6.1. 

# 1. Install Py-Uio
    sudo apt update

    sudo apt install python3-pip python3-setuptools python3-wheel

    pip3 install --src . -e 'git+https://github.com/mvduin/py-uio.git#egg=py-uio'

# 2. Configure /boot/uEnv.txt 
<em> Disable the Audio, Video, and HDMI ports, and enable the timer outputs </em>


    sudo nano /boot/uEnv.txt


Uncomment:

<em> Note: Do NOT uncomment disable_uboot_overlay_emmc=1 </em>

* disable_uboot_overlay_video=1
* disable_uboot_overlay_audio=1
* disable_uboot_overlay_wireless=1
* disable_uboot_overlay_adc=1
* uboot_overlay_pru=/lib/firmware/AM335X-PRU-UIO-00A0.dtbo

Add:
* uboot_overlay_addr0=/lib/firmware/BB-PWM-TIMER-P8.07.dtbo
* uboot_overlay_addr1=/lib/firmware/BB-PWM-TIMER-P8.08.dtbo
* uboot_overlay_addr2=/lib/firmware/BB-PWM-TIMER-P8.09.dtbo
* uboot_overlay_addr3=/lib/firmware/BB-PWM-TIMER-P8.10.dtbo

Comment out:
* #uboot_overlay_pru=/lib/firmware/AM335X-PRU-RPROC-4-19-TI-00A0.dtbo

# 3. (for PWM 12.0 MHz Timer Output) In order to easily import/export mappings, add the entries to the rules file

    sudo nano /etc/udev/rules.d/81-pwm-noroot.rules

Add:

* SUBSYSTEM=="pwm", KERNELS=="dmtimer-pwm-4",  ENV{PWMCHIP_NAME}="timer4"

* SUBSYSTEM=="pwm", KERNELS=="dmtimer-pwm-5",  ENV{PWMCHIP_NAME}="timer5"

* SUBSYSTEM=="pwm", KERNELS=="dmtimer-pwm-6",  ENV{PWMCHIP_NAME}="timer6"

* SUBSYSTEM=="pwm", KERNELS=="dmtimer-pwm-7",  ENV{PWMCHIP_NAME}="timer7"

Then in a terminal, write

    sudo reboot

# 4. Add all necessary files
Add the required files, including IOTest.pru0.c, prugpio.h, and io-test.py.

On the BeagleBone Black, place IORun.pru0.c and prugpio.h in /var/lib/cloud9/BeagleBone/Black/pru

On the ethernet host, place TCPServer.py

On the BeagleBone Black, clone the py-uio repo 

    git clone https://github.com/mvduin/py-uio.git

Place io-run.py in /var/lib/cloud9/py-uio/pru-examples/

# 5. Set up Ethernet between Host and BeagleBone Black
* Set a static IP for the beagle. To do this in the terminal, you can enter:
     sudo nano /etc/network/interfaces

Then Uncomment "auto eth0"
Add under "auto eth0"

    iface eth0 inet static
    address 192.168.0.25
    netmask 255.255.255.0
Finally,
sudo reboot

To set up the ethernet for a Cymhost virtual machine, the steps are as follows:
* Open Cymhost
* Go to Activities > Settings
* Go to Network Settings, select the cog next to Wired Connection
* Navigate to IPv4 
* Select manual for ipv4 method
* Input 192.168.0.30 for Address and 255.255.255.0 for netmask
* Select Apply
* For Oracle VM VirutalBox, go to the Machine Settings 
* Select Network button
* Attatched to Bridged Adapter
* Select the network Adapter
* Save

To remove and configure the firewall for the Cymhost Redhat linux distribution, you can follow this:
* Go to Terminal
* systemctl status firewalld
* systemctl stop firewalld
* systemctl disable firewalld
* systemctl status firewalld - Check that the fire wall is disabled

Finally, to test the connection, from the BeagleBone Black terminal, enter ping 192.168.0.30 in the terminal.
To set up the TCP/IP server, run the TCPClient.py on the Ethernet host
# 6. Output 12 MHz Clock, Make PRU code and run py-uio client
In a terminal, write the following

    cd /dev/pwm/timer7

    echo 83 > period

    echo 41 > duty_cycle

    echo 1 > enable

    cd /var/lib/cloud9/BeagleBone/Black/pru

    export TARGET=IORun.pru0

    make

    cp /lib/firmware/am335x-pru0-fw /var/lib/cloud9/py-uio/pru-examples/fw

    cd /var/lib/cloud9/py-uio/pru-examples

    ./io-run.py 


For the IO RT Run Test run the following

    cd /dev/pwm/timer7

    echo 83 > period

    echo 41 > duty_cycle

    echo 1 > enable

    cd /var/lib/cloud9/BeagleBone/Black/pru

    export TARGET=IORTRunTest.pru0

    make

    cp /lib/firmware/am335x-pru0-fw /var/lib/cloud9/py-uio/pru-examples/fw

    cd /var/lib/cloud9/py-uio/pru-examples

    ./io-run.py 
