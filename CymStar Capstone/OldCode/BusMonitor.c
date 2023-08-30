/*
 *	Bus Monitor Mode is used to read the HI-1575 pins
 *	to determine the 1553 traffic on the bus.
*/

#include <stdio.h>
#include "prugpio.h"
#include "1553_Word_Handler.pru0.c"
#include "TCP_Client.c"



int main() {
    pulse_MR();
    if (read_RCVA()){
        
    }
        return 0;
}