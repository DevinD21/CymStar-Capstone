////////////////////////////////////////
//	IOTest.pru0.c
//	Blinks all pins set up as outputs by writing to memory using the PRU on the Beaglebone Black
//	Wiring:	Probe the pins with an LED, Multimeter, or Oscilloscope to detect the switching
//	Setup:	Set the direction to out on all pins
//	See:	prugpio.h to see to which ports the P8 and P9 pins are attached
//	PRU:	pru0
////////////////////////////////////////
#include <stdint.h>
#include <stddef.h>
#include <pru_cfg.h>
#include "resource_table_empty.h"
#include "prugpio.h"
#define PRU_SHARED_MEM_ADDR 0x00010000
#define second 200000000
#define TEST_STATUS 		999
#define IDLE_STATUS 		1000
#define CW_RECEIVE_STATUS	1010
#define SW1_TRANSMIT_STATUS 1020
#define DW_TRANSMIT_STATUS	1030
#define DW_RECEIVE_STATUS	1040
#define SW2_TRANSMIT_STATUS 1050
#define GPIO_0_OUT shared_mem[0]
#define GPIO_1_OUT shared_mem[1]
#define GPIO_2_OUT shared_mem[2]
#define STATUS_OUT shared_mem[3]
#define RXDATA_OUT shared_mem[4]
#define TXDATA_IN shared_mem[5]
#define ERRMSG_OUT shared_mem[6]
#define CMDMSG_IN shared_mem[7]
#define RXTICK_OUT shared_mem[8]
#define RCVA_OUT shared_mem[9]
#define SAM_OUT shared_mem[10]
#define RTADDRESS_IN shared_mem[11]
#define RCVA P9_14
#define REG P8_15
#define RW P9_16
#define STRBn P8_17
#define MR P9_18
#define SYNC P8_18
#define D15 P8_35
#define D14 P8_43
#define D13 P8_33
#define D12 P9_30
#define D11 P8_31
#define D10 P8_41
#define D9  P8_29
#define D8  P9_28 //P9_30 // P9_28
#define D7  P8_27
#define D6  P8_39
#define D5  P8_25
#define D4  P9_26
#define D3  P8_23
#define D2  P9_24
#define D1  P8_21
#define D0  P9_22
//#define CLK P8_8
#define CHBCHA P9_12
#define ERROR P8_37
void all_on()
{
	gpio1[GPIO_SETDATAOUT] = P8_15;
	gpio0[GPIO_SETDATAOUT] |= P8_17;
	gpio0[GPIO_SETDATAOUT] |= P8_19;
	gpio1[GPIO_SETDATAOUT] |= P8_21;
	gpio1[GPIO_SETDATAOUT] |= P8_23;
	gpio1[GPIO_SETDATAOUT] |= P8_25;
	gpio2[GPIO_SETDATAOUT] |= P8_27;
	gpio2[GPIO_SETDATAOUT] |= P8_29;
	gpio0[GPIO_SETDATAOUT] |= P8_31;
	gpio0[GPIO_SETDATAOUT] |= P8_33;
	gpio0[GPIO_SETDATAOUT] |= P8_35;
	gpio2[GPIO_SETDATAOUT] |= P8_37;
	gpio2[GPIO_SETDATAOUT] |= P8_39;
	gpio2[GPIO_SETDATAOUT] |= P8_41;
	gpio1[GPIO_SETDATAOUT] |= P9_12;
	gpio1[GPIO_SETDATAOUT] |= P9_14;
	gpio1[GPIO_SETDATAOUT] |= P9_16;
	gpio0[GPIO_SETDATAOUT] |= P9_18;
	gpio0[GPIO_SETDATAOUT] |= P9_19;
	gpio0[GPIO_SETDATAOUT] |= P9_22;
	gpio0[GPIO_SETDATAOUT] |= P9_24;
	gpio0[GPIO_SETDATAOUT] |= P9_26;
	gpio3[GPIO_SETDATAOUT] |= P9_28;
	gpio3[GPIO_SETDATAOUT] |= P9_30;
}
void all_off()
{
	gpio1[GPIO_CLEARDATAOUT] |= P8_15;
	gpio0[GPIO_CLEARDATAOUT] |= P8_17;
	gpio0[GPIO_CLEARDATAOUT] |= P8_19;
	gpio1[GPIO_CLEARDATAOUT] |= P8_21;
	gpio1[GPIO_CLEARDATAOUT] |= P8_23;
	gpio1[GPIO_CLEARDATAOUT] |= P8_25;
	gpio2[GPIO_CLEARDATAOUT] |= P8_27;
	gpio2[GPIO_CLEARDATAOUT] |= P8_29;
	gpio0[GPIO_CLEARDATAOUT] |= P8_31;
	gpio0[GPIO_CLEARDATAOUT] |= P8_33;
	gpio0[GPIO_CLEARDATAOUT] |= P8_35;
	gpio2[GPIO_CLEARDATAOUT] |= P8_37;
	gpio2[GPIO_CLEARDATAOUT] |= P8_39;
	gpio2[GPIO_CLEARDATAOUT] |= P8_41;
	gpio1[GPIO_CLEARDATAOUT] |= P9_12;
	gpio1[GPIO_CLEARDATAOUT] |= P9_14;
	gpio1[GPIO_CLEARDATAOUT] |= P9_16;
	gpio0[GPIO_CLEARDATAOUT] |= P9_18;
	gpio0[GPIO_CLEARDATAOUT] |= P9_19;
	gpio0[GPIO_CLEARDATAOUT] |= P9_22;
	gpio0[GPIO_CLEARDATAOUT] |= P9_24;
	gpio0[GPIO_CLEARDATAOUT] |= P9_26;
	gpio3[GPIO_CLEARDATAOUT] |= P9_28;
	gpio3[GPIO_CLEARDATAOUT] |= P9_30;
}
void all_input()
{
	gpio1[GPIO_DIR] |= P8_15;
	gpio0[GPIO_DIR] |= P8_17;
	gpio0[GPIO_DIR] |= P8_19;
	gpio1[GPIO_DIR] |= P8_21;
	gpio1[GPIO_DIR] |= P8_23;
	gpio1[GPIO_DIR] |= P8_25;
	gpio2[GPIO_DIR] |= P8_27;
	gpio2[GPIO_DIR] |= P8_29;
	gpio0[GPIO_DIR] |= P8_31;
	gpio0[GPIO_DIR] |= P8_33;
	gpio0[GPIO_DIR] |= P8_35;
	gpio2[GPIO_DIR] |= P8_37;
	gpio2[GPIO_DIR] |= P8_39;
	gpio2[GPIO_DIR] |= P8_41;
	gpio1[GPIO_DIR] |= P9_12;
	gpio1[GPIO_DIR] |= P9_14;
	gpio1[GPIO_DIR] |= P9_16;
	gpio0[GPIO_DIR] |= P9_18;
	gpio0[GPIO_DIR] |= P9_19;
	gpio0[GPIO_DIR] |= P9_22;
	gpio0[GPIO_DIR] |= P9_24;
	gpio0[GPIO_DIR] |= P9_26;
	gpio3[GPIO_DIR] |= P9_28;
	gpio3[GPIO_DIR] |= P9_30;
}
void all_output()
{
	gpio1[GPIO_DIR] &= ~P8_15;
	gpio0[GPIO_DIR] &= ~P8_17;
	gpio0[GPIO_DIR] &= ~P8_19;
	gpio1[GPIO_DIR] &= ~P8_21;
	gpio1[GPIO_DIR] &= ~P8_23;
	gpio1[GPIO_DIR] &= ~P8_25;
	gpio2[GPIO_DIR] &= ~P8_27;
	gpio2[GPIO_DIR] &= ~P8_29;
	gpio0[GPIO_DIR] &= ~P8_31;
	gpio0[GPIO_DIR] &= ~P8_33;
	gpio0[GPIO_DIR] &= ~P8_35;
	gpio2[GPIO_DIR] &= ~P8_37;
	gpio2[GPIO_DIR] &= ~P8_39;
	gpio2[GPIO_DIR] &= ~P8_41;
	gpio1[GPIO_DIR] &= ~P9_12;
	gpio1[GPIO_DIR] &= ~P9_14;
	gpio1[GPIO_DIR] &= ~P9_16;
	gpio0[GPIO_DIR] &= ~P9_18;
	gpio0[GPIO_DIR] &= ~P9_19;
	gpio0[GPIO_DIR] &= ~P9_22;
	gpio0[GPIO_DIR] &= ~P9_24;
	gpio0[GPIO_DIR] &= ~P9_26;
	gpio3[GPIO_DIR] &= ~P9_28;
	gpio3[GPIO_DIR] &= ~P9_30;
}
void test_status()
{
	STATUS_OUT = TEST_STATUS;
	
	all_output();
	all_on();
	__delay_cycles(second/2);		// Wait 1/2 second
	all_off();
	__delay_cycles(second/2);		// Wait 1/2 second
}
//	if (status == TEST_STATUS) test_status();
