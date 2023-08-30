// This file defines the GPIO port addresses and PRU address
// Addresses for the am35xx
#define GPIO0 0x44E07000 
#define GPIO1 0x4804C000
#define GPIO2 0x481AC000
#define GPIO3 0x481AE000
// USR LED bit positions
// GPIO 1
#define USR0 (1<<21)
#define USR1 (1<<22)
#define USR2 (1<<23)
#define USR3 (1<<24)
// The define a couple of GPIO pin addresses on Black
// GPIO0
#define P8_14 (1<<26) // gpio0[26]
#define P8_17 (1<<27) // gpio0[27]
#define P8_19 (1<<22) // gpio0[22]
#define P8_31 (1<<10) // gpio0[10]
#define P8_32 (1<<11) // gpio0[11]
#define P8_33 (1<<9)  // gpio0[9]
#define P8_35 (1<<8)  // gpio0[8]
#define P9_18 (1<<4)  // gpio0[4]
#define P9_19 (1<<13) // gpio0[13]
#define P9_22 (1<<2)  // gpio0[2]
#define P9_24 (1<<15) // gpio0[15]
#define P9_26 (1<<14) // gpio0[14]
// GPIO1
#define P8_15 (1<<15) // gpio1[15]
#define P8_16 (1<<14) // gpio1[14]
#define P8_20 (1<<31) // gpio1[31]
#define P8_21 (1<<30) // gpio1[30]
#define P8_22 (1<<5)  // gpio1[5]
#define P8_23 (1<<4)  // gpio1[4]
#define P8_24 (1<<1)  // gpio1[1]
#define P8_25 (1<<0)  // gpio1[0]
#define P8_26 (1<<29) // gpio1[29]
#define P9_12 (1<<28) // gpio1[28]
#define P9_14 (1<<18) // gpio1[18]
#define P9_16 (1<<19) // gpio1[19]
// GPIO2
#define P8_18 (1<<1)  // gpio2[1]
#define P8_27 (1<<22) // gpio2[22]
#define P8_28 (1<<24) // gpio2[24]
#define P8_29 (1<<23) // gpio2[23]
#define P8_30 (1<<25) // gpio2[25]
#define P8_34 (1<<17) // gpio2[17]
#define P8_37 (1<<14) // gpio2[14]
#define P8_39 (1<<12) // gpio2[12]
#define P8_41 (1<<10) // gpio2[10]
#define P8_43 (1<<8)  // gpio2[8]
// GPIO3
#define P9_28 (1<<17)  // gpio3[17]
#define P9_30 (1<<16)  // gpio3[16]
// Shared memory
#define AM33XX_DATARAM0_PHYS_BASE		0x4a300000
#define AM33XX_DATARAM1_PHYS_BASE		0x4a302000
#define AM33XX_PRUSS_SHAREDRAM_BASE		0x4a310000
// /4 to convert from byte address to word address
#define GPIO_DIR            0x134/4     // rw  direction: 0 = output, 1 = input
#define GPIO_CLEARDATAOUT	0x190/4     // Write 1 here to set a given bit    
#define GPIO_SETDATAOUT 	0x194/4     // A 1 here clears the corresponding bit
#define GPIO_DATAOUT		0x138/4     // For reading the GPIO registers
#define PRU_SHARED_MEM_ADDR 0x00010000
#define second 200000000
#define TEST_STATUS 		999
#define IDLE_STATUS 		1000
#define CW_RECEIVE_STATUS	1010
#define SW1_TRANSMIT_STATUS 1020
#define DW_TRANSMIT_STATUS	1030
#define DW_RECEIVE_STATUS	1040
#define SW2_TRANSMIT_STATUS 1050
#define TEST_TRANSMIT_STATUS 1060
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
#define SYNC_IN shared_mem[11]
//#define RTADDRESS_IN shared_mem
#define RCVA P9_14
#define REG P8_15
#define RW P9_16
#define STRBn P8_17
#define MR P9_18
#define SYNC P8_19
#define D0  P9_22
#define D1   P8_21
#define D2  P9_24
#define D3   P8_23
#define D4  P9_26
#define D5   P8_25
#define D6  P8_39
#define D7   P8_27
#define D8  P9_28 //P9_30 // P9_28
#define D9   P8_29
#define D10 P8_41
#define D11  P8_31
#define D12 P9_30
#define D13  P8_33
#define D14 P8_43
#define D15  P8_35
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
//  Input Pin
int read_RCVA()
{
	gpio1[GPIO_DIR] |= (REG);
	return ((gpio1[GPIO_DATAOUT] >> 15) & 1);
}
// Output Pin
/*
void write_REG(int high)
{
	if (high) gpio1[GPIO_SETDATAOUT] = P8_15;
	else gpio1[GPIO_CLEARDATAOUT] = P8_15;
}
*/
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
void write_sync(int high)  //gpio0[22]
{
	gpio2[GPIO_DIR] &= ~SYNC;
	if (high) gpio2[GPIO_SETDATAOUT] = SYNC;
	else gpio2[GPIO_CLEARDATAOUT] = SYNC;
}
int read_sync()
{ 
	gpio0[GPIO_DIR] |= SYNC;
	return ((gpio0[GPIO_DATAOUT] >> 22) & 1);
}
// Bidirectional Bus
void write_data(int data)
{
	//gpio0[GPIO_DIR] &= ~(D15 | D13 | D11 | D4 | D2 | D0);
	//D13 would be here, but D13 is shorted to STRBn, which completely stops HI-1575 chip from happening
	gpio0[GPIO_DIR] &= ~(D15  | D11 | D4 | D2 | D0);
	gpio1[GPIO_DIR] &= ~(D5 | D3 | D1);
	gpio2[GPIO_DIR] &= ~(D14 | D10 | D9 | D7 | D6 );
	gpio3[GPIO_DIR] &= ~(D8 | D12);
	
	if ((data >> 15) & 1) gpio0[GPIO_SETDATAOUT] = D15;
	else gpio0[GPIO_CLEARDATAOUT] = D15;
	
	if ((data >> 14) & 1) gpio2[GPIO_SETDATAOUT] = D14;
	else gpio2[GPIO_CLEARDATAOUT] = D14;
	
	// if ((data >> 13) & 1) gpio0[GPIO_SETDATAOUT] = D13;
	// else gpio0[GPIO_CLEARDATAOUT] = D13;
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
	data |= (((gpio3[GPIO_DATAOUT] >> 16) & 1) << 12); 
	data |= (((gpio0[GPIO_DATAOUT] >> 10) & 1) << 11);
	data |= (((gpio2[GPIO_DATAOUT] >> 10) & 1) << 10);
	data |= (((gpio2[GPIO_DATAOUT] >> 23) & 1) << 9 );
	data |= (((gpio3[GPIO_DATAOUT] >> 17) & 1) << 8 ); 
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
	int samdata;
	pulse_MR();
	__delay_cycles(second/1000);
	write_RW(1);
	write_REG(1);
	__delay_cycles(5);
	write_STRBn(0);
	__delay_cycles(second/1000000); // Wait a moment
	samdata = read_data();
	write_STRBn(1);
	return samdata;
}
// Pulse STRB low while 
void write_SAM()
{
	write_RW(0);
	write_REG(1);
	__delay_cycles(5);
	write_STRBn(0);
	__delay_cycles(second/1000000); // Wait a moment
	write_STRBn(1);
}
// Pulse STRB low while R/W is low and REG is low
void signal_send()
{
	write_RW(0);
	write_REG(0);
	__delay_cycles(5);
	write_STRBn(0);
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
