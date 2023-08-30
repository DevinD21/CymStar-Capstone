////////////////////////////////////////
//	cape_test_gpio.h
//	sets up the gpio pins on the BBB to interface with the HI-1575 chip based on our design
// defines extra pins used for the LED and breadboard
////////////////////////////////////////

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
// defined according to table 12 of BBB system reference manual
// GPIO0
#define P8_17 (1<<27) // gpio0[27] 
#define P8_19 (1<<22) // gpio0[22] 
#define P8_31 (1<<10) // gpio0[10] 
#define P8_33 (1<<9)  // gpio0[9] 
#define P9_18 (1<<4)  // gpio0[4] might be 0[5]
#define P9_22 (1<<2)  // gpio0[2] 
#define P9_24 (1<<15) // gpio0[15] 
#define P9_26 (1<<14) // gpio0[14] 
#define P8_13 (1<<23) // gpio0[23] 

// GPIO1
#define P8_15 (1<<15) // gpio1[15] 
#define P8_21 (1<<30) // gpio1[30] 
#define P9_12 (1<<28) // gpio1[28]
#define P8_23 (1<<4)  //gpio1[4]
#define P8_25 (1<<0)  //gpio1[0]
#define P9_16 (1<<19)  //gpio1[19]
#define P8_11 (1<<13) // gpio1[13]
#define P8_5  (1<<2) // gpio1[2] 
#define P8_3  (1<<6) // gpio1[6] 

// GPIO2
#define P8_27 (1<<22) // gpio2[22] 
#define P8_29 (1<<23) // gpio2[23] 
#define P8_9  (1<<5) // gpio2[5] 
#define P8_7  (1<<2) // gpio2[2] 
#define P8_39 (1<<12) // gpio2[12]
//#define P8_43 (1<<8)  // gpio2[8]


// GPIO3
#define P9_30 (1<<16)  // gpio3[16] 
#define P9_28 (1<<17)  //gpio3[17]

//#define RTADDRESS_IN shared_mem
// renaming BBB pins to 1575 pin names
#define D15 P8_33
#define D14 P8_31
#define D13 P8_29 
#define D12 P8_27
#define D11 P8_25
#define D10 P8_23
#define D9 P8_21
#define D8 P8_19 
#define D7 P8_17
#define D6 P8_15
#define D5 P8_13 
#define D4 P8_11 
#define D3 P8_9 
#define D2 P8_7 
#define D1 P8_5 
#define D0 P8_3 
#define CHACHB P9_12 
#define ERROR P9_16 
#define MR P9_18 
#define RCVA  P9_22 
#define REG  P9_24 
#define RW  P9_26 
#define STRB  P9_28 
#define SYNC P9_30 

// Shared memory
#define AM33XX_DATARAM0_PHYS_BASE		0x4a300000
#define AM33XX_DATARAM1_PHYS_BASE		0x4a302000
#define AM33XX_PRUSS_SHAREDRAM_BASE		0x4a310000

#endif
// /4 to convert from byte address to word address
#define GPIO_DIR            0x134/4     // rw  direction: 0 = output, 1 = input
#define GPIO_CLEARDATAOUT	0x190/4     // A 1 here clears the corresponding bit
#define GPIO_SETDATAOUT 	0x194/4     // Write 1 here to set a given bit 
#define GPIO_DATAOUT		0x138/4    // For reading the GPIO registers

// define address for shared memory
#define PRU_SHARED_MEM_ADDR 0x00010000
#define millisecond 200000 // time for a millisecond (.001 seconds)
#define second 200000000

#define RCVA_OUT shared_mem[9]
volatile register unsigned int __R30;
volatile register unsigned int __R31;
uint32_t *gpio3 = (uint32_t *)GPIO3;
uint32_t *gpio2 = (uint32_t *)GPIO2;
uint32_t *gpio1 = (uint32_t *)GPIO1;
uint32_t *gpio0 = (uint32_t *)GPIO0;
volatile uint32_t* shared_mem = (volatile uint32_t *) PRU_SHARED_MEM_ADDR;
