////////////////////////////////////////////////////////////////////////////////
// BusMonitor.c
//
// This file implements the Bus Monitor Mode for the 1553 data bus.
// Normal operation will use the functions in the included files section
// to gather and pack the data into a character array and send those
// packets using TCP protocol to the host computer.
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdint.h>
#include "TCP_Client.c"
#include "BusMonitor_init_pins.h"
#include "1575_functions_gpio.c"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
//#include "busmonitortest.c" // Used for debugging purposes & Check state of gpio's

extern int createSocket();
extern void connectSocket(int socketfd);
extern void socketSend(int socketfd);
extern void socketRecv(int socketfd);
extern int tcpProtocol();
extern int read_gpio(int GPIOnumber);

uint8_t D15_12[4] = { D12, D13, D14, D15 };
uint8_t D11_8[4] = { D8, D9, D10, D11 };
uint8_t D7_4[4] = { D4, D5, D6, D7 };
uint8_t D3_0[4] = { D0, D1, D2, D3 };

uint8_t packet[5];

uint8_t* wordToTCPPacket(void) {
    /*
        Use this function to convert the data word [16 bits] into two
        halves [8 bits] that can be converted into a 'char' type [8 bits]
        for use in the TCP protocol.
    */
    packet[0] = 0;
    packet[1] = 0;
    packet[2] = 0;
    packet[3] = 0;
    packet[4] = 0;

    // Use bit shifting to assing each D-pin to its
    // corresponding location in the data word.
    packet[0] = read_gpio(SYNC);
    for (int i = 0; i < 4; i++) { // higher order bits (8-15)
        packet[1] |= (read_gpio(D15_12[i]) << i);
    }
    for (int i = 0; i < 4; i++) { // lower order bits (0-7)
        packet[2] |= (read_gpio(D11_8[i]) << i);
    }
    for (int i = 0; i < 4; i++) { // lower order bits (0-7)
        packet[3] |= (read_gpio(D7_4[i]) << i);
    }
    for (int i = 0; i < 4; i++) { // lower order bits (0-7)
        packet[4] |= (read_gpio(D3_0[i]) << i);
    }
    return packet;
}

void ExportToServer() {

    return;
}

FILE* f;

int main() {
    initializePins();
    int state = 0;
    int incomingDataWord = 0;
    int exitTrigger = 0;

    //  Reset the HI-1575 by pulsing the MR pin.
    pulse_mr();
    while (!exitTrigger) {
        if (read_gpio(RCVA)) {
            pulse_strb(); //  Pulse STRB to read (requires R/W = High, REG = LOW)
            // sleep(1.0);
            if (read_gpio(SYNC)) {
                int sync = read_gpio(SYNC);
                uint8_t* incomingWord = wordToTCPPacket();
                printf("TCPPacket = 0x%x%x%x%x%x\n", incomingWord[0], //SYNC bit
                    incomingWord[1], incomingWord[2],  //D15-D8
                    incomingWord[3], incomingWord[4]); //D7-0

                sleep(1);
            }

        }
        //exitTrigger = ExitBusMonitorMode();

    }

    return 0;
}