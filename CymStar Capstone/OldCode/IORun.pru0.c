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
volatile register unsigned int __R30;
volatile register unsigned int __R31;
uint32_t *gpio3 = (uint32_t *)GPIO3;
uint32_t *gpio2 = (uint32_t *)GPIO2;
uint32_t *gpio1 = (uint32_t *)GPIO1;
uint32_t *gpio0 = (uint32_t *)GPIO0;
volatile uint32_t* shared_mem = (volatile uint32_t *) PRU_SHARED_MEM_ADDR;
struct SubAdr {
	uint8_t address;
	uint8_t word_count;
	uint16_t data[32];
};
typedef struct SubAdr SubAdr;
struct RemoteTerminal {
	uint8_t address;
	SubAdr rxaddresses[30];
	SubAdr txaddresses[30];
};
int status = IDLE_STATUS;
far struct RemoteTerminal rtdata __attribute__((location(0x11000))) = {};
//  Input Pin
int read_RCVA()
{
	gpio1[GPIO_DIR] |= (RCVA);
	return ((gpio1[GPIO_DATAOUT] >> 18) & 1);
}
// Output Pin
void write_REG(int high)
{
	if (high) gpio1[GPIO_SETDATAOUT] = P8_15;
	else gpio1[GPIO_CLEARDATAOUT] = P8_15;
}
// Output Pin
void write_RW(int high)
{
	if (high) gpio1[GPIO_SETDATAOUT] = P9_16;
	else gpio1[GPIO_CLEARDATAOUT] = P9_16;
}
// Output Pin
void write_STRBn(int high)
{
	if (high) gpio0[GPIO_SETDATAOUT] = STRBn;
	else gpio0[GPIO_CLEARDATAOUT] = STRBn;
}
// Output Pin
void write_MR(int high)
{
	if (high) gpio0[GPIO_SETDATAOUT] = MR; // Set MR High
	else gpio0[GPIO_CLEARDATAOUT] = MR; // Set MR Low
}
// Bidirectional Pin
void write_sync(int high)
{
	gpio2[GPIO_DIR] &= ~SYNC;
	if (high) gpio2[GPIO_SETDATAOUT] = SYNC;
	else gpio2[GPIO_CLEARDATAOUT] = SYNC;
}
int read_sync()
{
	gpio2[GPIO_DIR] |= SYNC;
	return ((gpio2[GPIO_DATAOUT] >> 1) & 1);
}
// Bidirectional Bus
void write_data(int data)
{
	gpio0[GPIO_DIR] &= ~(D15 | D13 | D11 | D4 | D2 | D0);
	gpio1[GPIO_DIR] &= ~(D5 | D3 | D1);
	gpio2[GPIO_DIR] &= ~(D14 | D10 | D9 | D7 | D6 );
	gpio3[GPIO_DIR] &= ~(D8 | D12);
	
	if ((data >> 15) & 1) gpio0[GPIO_SETDATAOUT] = D15;
	else gpio0[GPIO_CLEARDATAOUT] = D15;
	
	if ((data >> 14) & 1) gpio2[GPIO_SETDATAOUT] = D14;
	else gpio2[GPIO_CLEARDATAOUT] = D14;
	
	if ((data >> 13) & 1) gpio0[GPIO_SETDATAOUT] = D13;
	else gpio0[GPIO_CLEARDATAOUT] = D13;
	if ((data >> 12) & 1) gpio3[GPIO_SETDATAOUT] = D12;
	else gpio3[GPIO_CLEARDATAOUT] = D12;
	
	if ((data >> 11) & 1) gpio0[GPIO_SETDATAOUT] = D11;
	else gpio0[GPIO_CLEARDATAOUT] = D11;
	
	if ((data >> 10) & 1) gpio2[GPIO_SETDATAOUT] = D10;
	else gpio2[GPIO_CLEARDATAOUT] = D10;
	
	if ((data >> 9) & 1) gpio2[GPIO_SETDATAOUT] = D9;
	else gpio2[GPIO_CLEARDATAOUT] = D9;
	
	if ((data >> 8) & 1) gpio3[GPIO_SETDATAOUT] = D8;
	else gpio3[GPIO_CLEARDATAOUT] = D8;
	
	if ((data >> 7) & 1) gpio2[GPIO_SETDATAOUT] = D7;
	else gpio0[GPIO_CLEARDATAOUT] = D7;
	
	if ((data >> 6) & 1) gpio2[GPIO_SETDATAOUT] = D6;
	else gpio2[GPIO_CLEARDATAOUT] = D6;
	
	if ((data >> 5) & 1) gpio1[GPIO_SETDATAOUT] = D5;
	else gpio1[GPIO_CLEARDATAOUT] = D5;
	
	if ((data >> 4) & 1) gpio0[GPIO_SETDATAOUT] = D4;
	else gpio0[GPIO_CLEARDATAOUT] = D4;
	
	if ((data >> 3) & 1) gpio1[GPIO_SETDATAOUT] = D3;
	else gpio1[GPIO_CLEARDATAOUT] = D3;
	
	if ((data >> 2) & 1) gpio0[GPIO_SETDATAOUT] = D2;
	else gpio0[GPIO_CLEARDATAOUT] = D2;
	
	if ((data >> 1) & 1) gpio1[GPIO_SETDATAOUT] = D1;
	else gpio1[GPIO_CLEARDATAOUT] = D1;
	
	if ((data >> 0) & 1) gpio0[GPIO_SETDATAOUT] = D0;
	else gpio0[GPIO_CLEARDATAOUT] = D0;
}
int read_data()
{	
	gpio0[GPIO_DIR] |= (D15 | D13 | D11 | D4 | D2 | D0);
	gpio1[GPIO_DIR] |= (D5 | D3 | D1);
	gpio2[GPIO_DIR] |= (D14 | D10 | D9 | D7 | D6 );
	gpio3[GPIO_DIR] |= (D8 | D12);
	
	int data = 0;
	data |= (((gpio0[GPIO_DATAOUT] >> 8 ) & 1) << 15);
	data |= (((gpio2[GPIO_DATAOUT] >> 8 ) & 1) << 14);
	data |= (((gpio0[GPIO_DATAOUT] >> 9 ) & 1) << 13);
	data |= (((gpio3[GPIO_DATAOUT] >> 16) & 1) << 12); //WHAT
	data |= (((gpio0[GPIO_DATAOUT] >> 10) & 1) << 11);
	data |= (((gpio2[GPIO_DATAOUT] >> 10) & 1) << 10);
	data |= (((gpio2[GPIO_DATAOUT] >> 23) & 1) << 9 );
	data |= (((gpio3[GPIO_DATAOUT] >> 17) & 1) << 8 ); //WHAT
	data |= (((gpio2[GPIO_DATAOUT] >> 22) & 1) << 7 );
	data |= (((gpio2[GPIO_DATAOUT] >> 12) & 1) << 6 );
	data |= (((gpio1[GPIO_DATAOUT] >> 0 ) & 1) << 5 );
	data |= (((gpio0[GPIO_DATAOUT] >> 14) & 1) << 4 );
	data |= (((gpio1[GPIO_DATAOUT] >> 4 ) & 1) << 3 );
	data |= (((gpio0[GPIO_DATAOUT] >> 15) & 1) << 2 );
	data |= (((gpio1[GPIO_DATAOUT] >> 30) & 1) << 1 ); 
	data |= (((gpio0[GPIO_DATAOUT] >> 2 ) & 1) << 0 ); 
	
	return data;
}
// Output Pin 
void writeCHBCHA() // gpio1[28]
{
	gpio1[GPIO_CLEARDATAOUT] = CHBCHA;
}
// Input Pin
int readERROR() // P8_37 gpio2[14]
{
	gpio2[GPIO_DIR] |= ERROR;
	return (gpio2[GPIO_DATAOUT] >> 14) & 1;
}
void pulse_MR()
{
	write_MR(1);
	__delay_cycles(second/1000);	  // Wait a moment
	write_MR(0);
	__delay_cycles(second/1000); 	  // Wait a moment
}
int read_SAM()
{
	write_STRBn(0);
	write_RW(1);
	write_REG(0);
	__delay_cycles(second/1000000); // Wait a moment
	return read_data();
}
// Pulse STRB low while 
void write_SAM()
{
	write_STRBn(0);
	write_RW(0);
	write_REG(1);
	__delay_cycles(second/1000000); // Wait a moment
	write_STRBn(1);
}
// Pulse STRB low while R/W is low and REG is low
void signal_send()
{
	write_STRBn(0);
	write_RW(0);
	write_REG(0);
	__delay_cycles(second/1000000); // Wait a moment
	write_STRBn(1);
}
// Read SAM and wait for bit 15 to go to 0
void wait_for_transmit()
{
	while (((read_SAM() >> 15) & 1) == 1)
	{
		// Wait here
	}
}
void idle_status()
{
	STATUS_OUT = IDLE_STATUS;
	RCVA_OUT = read_RCVA();
	ERRMSG_OUT = readERROR();
	if (CMDMSG_IN == TEST_STATUS)
	{
		status = TEST_STATUS;
	}
	if(read_RCVA() == 1)
	{
		status = CW_RECEIVE_STATUS;
	} 
}
int wordcountreq;
int subaddreq;
void cw_receive_status()
{
	STATUS_OUT = CW_RECEIVE_STATUS;
	RXTICK_OUT = RXTICK_OUT + 1;
	ERRMSG_OUT = readERROR();
	// The data word is read by pulsing STRB low, while R/W is high and REG is low
	write_RW(1); 
	write_REG(0);
	write_STRBn(0);
	int sync = read_sync();
	int message = read_data();
	write_STRBn(1);
	
	// CommandWord[0:4]  = RT Address CommandWord[5]     = T/R
	// CommandWord[6:10] = Subaddress CommandWord[11:16] = Word Count
	if (sync == 1) // SYNC: 1 = command sync, 0 = data sync
	{
		// Check if message corresponds to the RT subaddress
		if ((message & 0x1F) == rtdata.address)
		{
			subaddreq = ((message >> 6) & 0x1F);
			wordcountreq = ((message >> 11) & 0x1F);
			if (wordcountreq == 0) wordcountreq = 32;
			if ((message >> 5) & 1 == 1) // Transmit = 1
			{
				// If RT has a subaddress at subaddreq and the wordcountreq is correct
				if (rtdata.txaddresses[subaddreq].word_count == wordcountreq)
				{
					status = SW1_TRANSMIT_STATUS;
				}
				else
				{
					status = IDLE_STATUS;
				}
			}
			else	// Receive
			{
				// If RT has a subaddress at subaddreq and the wordcountreq is correct
				if (rtdata.rxaddresses[subaddreq].word_count == wordcountreq)
				{
					status = DW_RECEIVE_STATUS;
				}
				else 
				{
					status = IDLE_STATUS;
				}
			}
		}
		else 
		{
			status = IDLE_STATUS;
		}
		
	}
	else
	{
		status = IDLE_STATUS;
	}
	
	// RXDATA_OUT for bus monitoring
	RXDATA_OUT = (sync << 16 | message) ;
}
void status_word_1_transmit_status()
{
	STATUS_OUT = SW1_TRANSMIT_STATUS;
	//Data words to be transmitted on the MIL-STD-1553 data bus are written to the TX register by pulsing STRB low while R/W is low and REG is low.
	uint16_t syncword = (rtdata.address & 0x1F) | (readERROR() << 6);
	write_RW(0); 
	write_REG(0);
	write_STRBn(0);
	write_sync(1);
	write_data(syncword);
	write_STRBn(1);
	
	status = DW_TRANSMIT_STATUS;
}
void data_word_transmit_status()
{
	STATUS_OUT = DW_TRANSMIT_STATUS;
	
	//Data words to be transmitted on the MIL-STD-1553 data bus are written to the TX register by pulsing STRB low while R/W is low and REG is low.
	int count = 0;
	for (count = 0; count < wordcountreq; count++)
	{
		uint16_t message = (rtdata.txaddresses[subaddreq].data[count] & 0xFFFF);
		write_RW(0); 
		write_REG(0);
		write_STRBn(0);
		write_sync(1);
		write_data(message);
		write_STRBn(1);
	}
	status = IDLE_STATUS;
}
void data_word_receive_status()
{
	STATUS_OUT = DW_RECEIVE_STATUS;
	
	// The data word is read by pulsing STRB low, while R/W is high and REG is low
	int count;
	for (count = 0; count < wordcountreq; count++)
		while (!read_RCVA()) { } // Wait for the receive to happen 
		write_RW(1); 
		write_REG(0);
		write_STRBn(0);
		int sync = read_sync();
		int message = read_data();
		write_STRBn(1);
		rtdata.rxaddresses[subaddreq].data[count] = message;
		
	status = SW2_TRANSMIT_STATUS;
}
void status_word_2_transmit_status()
{
	STATUS_OUT == SW2_TRANSMIT_STATUS;
	//Data words to be transmitted on the MIL-STD-1553 data bus are written to the TX register by pulsing STRB low while R/W is low and REG is low.
	uint16_t syncword = (rtdata.address & 0x1F) | (readERROR() << 6);
	write_RW(0); 
	write_REG(0);
	write_STRBn(0);
	write_sync(1);
	write_data(syncword);
	write_STRBn(1);
	
	status = IDLE_STATUS;
}
void main(void) {
	/* Clear SYSCFG[STANDBY_INIT] to enable OCP master port */
	CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;
	volatile int* shared_mem = (volatile int *) PRU_SHARED_MEM_ADDR;
	// initialize shared memory
	GPIO_0_OUT = 0;  // gpio_0 Data to output from pru
	GPIO_1_OUT = 0;  // gpio_1 Data to output from pru
	GPIO_2_OUT = 0;  // gpio_2 Data to output from pru
	STATUS_OUT = 0;  // status Data to output from pru
	RXDATA_OUT = 0;  // rx data message from pru 
	//TXDATA_IN      // tx data message from client 
	ERRMSG_OUT = 0;  // error Data to output from pru
	//CMDMSG_IN      // command message received from client
	RXTICK_OUT = 0;  // rx data word count from pru
	RCVA_OUT = 0;
	SAM_OUT = 0;
	
	// set CHBCHA to 0 (CHA! = 0, so CHA = 1)
	writeCHBCHA();
	write_STRBn(1);
	
	pulse_MR();
	SAM_OUT = read_SAM(); 
	write_SAM();
	
	while(1)
	{
		if (status == IDLE_STATUS) idle_status();
		if (status == CW_RECEIVE_STATUS) cw_receive_status();
		
		if (status == SW1_TRANSMIT_STATUS) status_word_1_transmit_status();
		if (status == DW_TRANSMIT_STATUS) data_word_transmit_status();
		
		if (status == DW_RECEIVE_STATUS) data_word_receive_status();
		if (status == SW2_TRANSMIT_STATUS) status_word_2_transmit_status();
	}
}
// Set direction of pins
#pragma DATA_SECTION(init_pins, ".init_pins")
#pragma RETAIN(init_pins)
const char init_pins[] =  
	"/sys/class/gpio/gpio26/direction\0out\0" \
	"/sys/class/gpio/gpio27/direction\0out\0" \
	"/sys/class/gpio/gpio22/direction\0out\0" \
	"/sys/class/gpio/gpio10/direction\0out\0" \
	"/sys/class/gpio/gpio11/direction\0out\0" \
	"/sys/class/gpio/gpio9/direction\0out\0"  \
	"/sys/class/gpio/gpio8/direction\0out\0"  \
	"/sys/class/gpio/gpio47/direction\0out\0" \
	"/sys/class/gpio/gpio46/direction\0out\0" \
	"/sys/class/gpio/gpio63/direction\0out\0" \
	"/sys/class/gpio/gpio62/direction\0out\0" \
	"/sys/class/gpio/gpio37/direction\0out\0" \
	"/sys/class/gpio/gpio36/direction\0out\0" \
	"/sys/class/gpio/gpio33/direction\0out\0" \
	"/sys/class/gpio/gpio32/direction\0out\0" \
	"/sys/class/gpio/gpio61/direction\0out\0" \
	"/sys/class/gpio/gpio65/direction\0out\0" \
	"/sys/class/gpio/gpio86/direction\0out\0" \
	"/sys/class/gpio/gpio88/direction\0out\0" \
	"/sys/class/gpio/gpio87/direction\0out\0" \
	"/sys/class/gpio/gpio89/direction\0out\0" \
	"/sys/class/gpio/gpio81/direction\0out\0" \
	"/sys/devices/platform/ocp/ocp:P8_15_pinmux/state\0gpio\0" \
	"/sys/devices/platform/ocp/ocp:P8_17_pinmux/state\0gpio\0" \
	"/sys/devices/platform/ocp/ocp:P8_19_pinmux/state\0gpio\0" \
	"/sys/devices/platform/ocp/ocp:P8_21_pinmux/state\0gpio\0" \
	"/sys/devices/platform/ocp/ocp:P8_23_pinmux/state\0gpio\0" \
	"/sys/devices/platform/ocp/ocp:P8_25_pinmux/state\0gpio\0" \
	"/sys/devices/platform/ocp/ocp:P8_27_pinmux/state\0gpio\0" \
	"/sys/devices/platform/ocp/ocp:P8_29_pinmux/state\0gpio\0" \
	"/sys/devices/platform/ocp/ocp:P8_31_pinmux/state\0gpio\0" \
	"/sys/devices/platform/ocp/ocp:P8_33_pinmux/state\0gpio\0" \
	"/sys/devices/platform/ocp/ocp:P8_35_pinmux/state\0gpio\0" \
	"/sys/devices/platform/ocp/ocp:P8_37_pinmux/state\0gpio\0" \
	"/sys/devices/platform/ocp/ocp:P8_39_pinmux/state\0gpio\0" \
	"/sys/devices/platform/ocp/ocp:P8_41_pinmux/state\0gpio\0" \
	"/sys/devices/platform/ocp/ocp:P9_12_pinmux/state\0gpio\0" \
	"/sys/devices/platform/ocp/ocp:P9_14_pinmux/state\0gpio\0" \
	"/sys/devices/platform/ocp/ocp:P9_16_pinmux/state\0gpio\0" \
	"/sys/devices/platform/ocp/ocp:P9_18_pinmux/state\0gpio\0" \
	"/sys/devices/platform/ocp/ocp:P9_19_pinmux/state\0gpio\0" \
	"/sys/devices/platform/ocp/ocp:P9_22_pinmux/state\0gpio\0" \
	"/sys/devices/platform/ocp/ocp:P9_24_pinmux/state\0gpio\0" \
	"/sys/devices/platform/ocp/ocp:P9_26_pinmux/state\0gpio\0" \
	"/sys/devices/platform/ocp/ocp:P9_28_pinmux/state\0gpio\0" \
	"/sys/devices/platform/ocp/ocp:P9_30_pinmux/state\0gpio\0" \
	"\0\0";
