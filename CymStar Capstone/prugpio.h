// This file defines the GPIO port addresses and PRU address

#if defined(CHIP) && defined(CHIP_IS_am57xx)
#warning "Found AI"
#else
#warning "Found am335x"


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
#define P8_17 (1<<27) // gpio0[27] 
#define P8_19 (1<<22) // gpio0[22] 
#define P8_31 (1<<10) // gpio0[10] 
#define P8_33 (1<<9)  // gpio0[9] 
#define P9_18 (1<<4)  // gpio0[4] ??? this might be 0[5]
#define P9_22 (1<<2)  // gpio0[2] 
#define P9_24 (1<<15) // gpio0[15] 
#define P9_26 (1<<14) // gpio0[14] 
#define P8_13 (1<<23) // gpio0[23] 
#define P8_11 (1<<13) // gpio1[13] 
#define P8_9  (1<<5) // gpio2[5] 
#define P8_7  (1<<2) // gpio2[2] 
#define P8_5  (1<<2) // gpio1[2] 
#define P8_3  (1<<6) // gpio1[6] 



// GPIO1
#define P8_15 (1<<15) // gpio1[15] 
#define P8_21 (1<<30) // gpio1[30] 
#define P9_12 (1<<28) // gpio1[28]
#define P9_14 (1<<18) // gpio1[18]
#define P8_23 (1<<4)  //gpio1[4]
#define P8_25 (1<<0)  //gpio1[0]
#define P9_16 (1<<19)  //gpio1[19]



// GPIO2

#define P8_27 (1<<22) // gpio2[22] 
#define P8_29 (1<<23) // gpio2[23] 

// GPIO3
#define P9_30 (1<<16)  // gpio3[16] 
#define P9_28 (1<<17)  //gpio3[17]


// Shared memory
#define AM33XX_DATARAM0_PHYS_BASE		0x4a300000
#define AM33XX_DATARAM1_PHYS_BASE		0x4a302000
#define AM33XX_PRUSS_SHAREDRAM_BASE		0x4a310000

#endif
// /4 to convert from byte address to word address
#define GPIO_DIR            0x134/4     // rw  direction: 0 = output, 1 = input
#define GPIO_CLEARDATAOUT	0x190/4     // Write 1 here to set a given bit    
#define GPIO_SETDATAOUT 	0x194/4     // A 1 here clears the corresponding bit
#define GPIO_DATAOUT		0x138/4     // For reading the GPIO registers

#define PRU_SHARED_MEM_ADDR 0x00010000
#define millisecond 200000 // time for a millisecond (.001 seconds)
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
#define D5 P8_13 
#define D4 P8_11 
#define D3 P8_9 
#define D2 P8_7 
#define D1 P8_5 
#define D0 P8_3 
#define CHACHB P9_12 
#define CLK P9_14 
#define D6 P8_15 
#define ERROR P9_16 
#define D7 P8_17 
#define MR P9_18 
#define D8 P8_19 
#define RCVA  P9_22 
#define D9   P8_21 
#define REG  P9_24 
#define D10   P8_23 
#define RW  P9_26 
#define D11   P8_25 
#define D12   P8_27 
#define STRB  P9_28 //P9_30 // P9_28 
#define D13   P8_29 
#define D14  P8_31 
#define SYNC P9_30 
#define D15  P8_33 

// all functions should match our pinouts
volatile register unsigned int __R30;
volatile register unsigned int __R31;
uint32_t *gpio3 = (uint32_t *)GPIO3;
uint32_t *gpio2 = (uint32_t *)GPIO2;
uint32_t *gpio1 = (uint32_t *)GPIO1;
uint32_t *gpio0 = (uint32_t *)GPIO0;
volatile uint32_t* shared_mem = (volatile uint32_t *) PRU_SHARED_MEM_ADDR;
