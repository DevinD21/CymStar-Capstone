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
struct SubAdr {
	volatile uint8_t address;
	volatile uint8_t word_count;
	volatile uint16_t data[32];
};
typedef struct SubAdr SubAdr;
struct RemoteTerminal {
	volatile uint8_t address;
	volatile SubAdr rxaddresses[30];
	volatile SubAdr txaddresses[30];
};
int status;
far struct RemoteTerminal rtdata __attribute__((location(0x11000))) = {};
void idle_status()
{
	STATUS_OUT = IDLE_STATUS;
	RCVA_OUT = 1;
	ERRMSG_OUT = readERROR();
	if (CMDMSG_IN == TEST_STATUS)
	{
		status = TEST_STATUS;
	}
	if(RCVA_OUT)
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
	int sync;
	sync = SYNC_IN;
	int message;
	message = TXDATA_IN;
	write_STRBn(1);
	
	// CommandWord[0:4]  = RT Address CommandWord[5]     = T/R
	// CommandWord[6:10] = Subaddress CommandWord[11:16] = Word Count
	if (sync == 1) // SYNC: 1 = command sync, 0 = data sync
	{
		ERRMSG_OUT = 2;
		// Check if message corresponds to the RT subaddress
		if (((message >> 11) & 0x1F) == 20)
		{
			ERRMSG_OUT = 2020;
			subaddreq = ((message >> 5) & 0x1F);
			wordcountreq = (message & 0x1F);
			if (wordcountreq == 0) wordcountreq = 32;
			if (((message >> 10) & 1 ) == 1) // Transmit = 1
			{
				ERRMSG_OUT = 3033;
				// If RT has a subaddress at subaddreq and the wordcountreq is correct
				GPIO_0_OUT = rtdata.txaddresses[subaddreq].word_count;
				GPIO_1_OUT = wordcountreq;
				GPIO_2_OUT = subaddreq;
				if (rtdata.txaddresses[subaddreq].word_count == wordcountreq)
				{
					status = SW1_TRANSMIT_STATUS;
				}
				else
				{
					ERRMSG_OUT = 3034;
					status = IDLE_STATUS;
				}
			}
			else	// Receive
			{
				ERRMSG_OUT = 4044;
				// If RT has a subaddress at subaddreq and the wordcountreq is 
				GPIO_0_OUT = rtdata.txaddresses[subaddreq].word_count;
				GPIO_1_OUT = wordcountreq;
				GPIO_2_OUT = subaddreq;
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
			ERRMSG_OUT = message;
			status = IDLE_STATUS;
		}
		
	}
	else
	{
		status = IDLE_STATUS;
	}
	
	// RXDATA_OUT for bus monitoring
	//RXDATA_OUT = (sync << 16 | message) ;
}
void status_word_1_transmit_status()
{
	STATUS_OUT = SW1_TRANSMIT_STATUS;
	//Data words to be transmitted on the MIL-STD-1553 data bus are written to the TX register by pulsing STRB low while R/W is low and REG is low.
	RXTICK_OUT = RXTICK_OUT + 1;
	uint16_t syncword = (rtdata.address & 0x1F) | (readERROR() << 6);
	write_RW(0); 
	write_REG(0);
	write_STRBn(0);
	write_sync(1);
	RXDATA_OUT = syncword; 
	write_STRBn(1);
	
	status = DW_TRANSMIT_STATUS;
}
int count = 0;
void data_word_transmit_status()
{
	STATUS_OUT = DW_TRANSMIT_STATUS;
	GPIO_0_OUT = rtdata.txaddresses[subaddreq].word_count;
	GPIO_1_OUT = wordcountreq;
	GPIO_2_OUT = subaddreq;
	
	//Data words to be transmitted on the MIL-STD-1553 data bus are written to the TX register by pulsing STRB low while R/W is low and REG is low.
	if (count < wordcountreq-1)
	{
		count++;
		uint16_t message = (rtdata.txaddresses[subaddreq].data[count] & 0xFFFF);
		write_RW(0); 
		write_REG(0);
		write_STRBn(0);
		write_sync(1);
		RXDATA_OUT = message;
		RXTICK_OUT = RXTICK_OUT + 1;
		write_STRBn(1);
	}
	else 
	{
		status = IDLE_STATUS;
		count = 0;
	}
}
void data_word_receive_status()
{
	STATUS_OUT = DW_RECEIVE_STATUS;
	// The data word is read by pulsing STRB low, while R/W is high and REG is low
	if (count < wordcountreq-1)
	{
		count++;
		while (!RCVA_OUT) { } // Wait for the receive to happen 
		write_RW(1); 
		write_REG(0);
		write_STRBn(0);
		int sync = SYNC_IN;
		int message = TXDATA_IN;
		RXTICK_OUT = RXTICK_OUT + 1;
		write_STRBn(1);
		rtdata.rxaddresses[subaddreq].data[count] = message;
	}
	else 
	{
		status = SW2_TRANSMIT_STATUS;
		count = 0;
	}
}
void status_word_2_transmit_status()
{
	STATUS_OUT == SW2_TRANSMIT_STATUS;
	RXTICK_OUT = RXTICK_OUT + 1;
	ERRMSG_OUT = 1341;
	//Data words to be transmitted on the MIL-STD-1553 data bus are written to the TX register by pulsing STRB low while R/W is low and REG is low.
	uint16_t statusword = (rtdata.address & 0x1F) | (readERROR() << 6);
	write_RW(0); 
	write_REG(0);
	write_sync(1);
	RXDATA_OUT = statusword;
	write_STRBn(0);
	write_STRBn(1);
	status = IDLE_STATUS;
}
void main(void) {
	/* Clear SYSCFG[STANDBY_INIT] to enable OCP master port */
	rtdata.address = 20;
	CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;
	volatile int* shared_mem = (volatile int *) PRU_SHARED_MEM_ADDR;
	// initialize shared memory
	STATUS_OUT = 0;  // status Data to output from pru
	RXDATA_OUT = 0;  // rx data message from pru sssss
	//TXDATA_IN      // tx data message from client 
	ERRMSG_OUT = 0;  // error Data to output from pru
	//CMDMSG_IN      // command message received from client
	RXTICK_OUT = 0;  // rx data word count from pru
	RCVA_OUT = 0;
	SAM_OUT = 0;
	SYNC_IN = 0;
	GPIO_0_OUT = 0;
	GPIO_1_OUT = 0;
	GPIO_2_OUT = 0;
	rtdata.txaddresses[1].word_count = 15;
	rtdata.rxaddresses[1].word_count = 13;
	// set CHBCHA to 0 (CHA! = 0, so CHA = 1)
	writeCHBCHA();
	write_STRBn(1);
	
	pulse_MR();
	SAM_OUT = read_SAM(); 
	status = CMDMSG_IN;
	while(1)
	{
		if (status == IDLE_STATUS) idle_status();
		else if (status == CW_RECEIVE_STATUS) cw_receive_status();
		else if (status == SW1_TRANSMIT_STATUS) status_word_1_transmit_status();
		else if (status == DW_TRANSMIT_STATUS) data_word_transmit_status();
		else if (status == DW_RECEIVE_STATUS) data_word_receive_status();
		else if (status == SW2_TRANSMIT_STATUS) status_word_2_transmit_status();
		__delay_cycles(second/2);
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
	"/sys/class/gpio/gpio51/direction\0out\0" \
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
