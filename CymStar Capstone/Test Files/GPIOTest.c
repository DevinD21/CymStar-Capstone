#include <stdint.h>
#include <stddef.h>


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

#define P8_25 (1<<0)  // gpio1[0]

// Shared memory
#define AM33XX_DATARAM0_PHYS_BASE		0x4a300000
#define AM33XX_DATARAM1_PHYS_BASE		0x4a302000
#define AM33XX_PRUSS_SHAREDRAM_BASE		0x4a310000

// /4 to convert from byte address to word address
#define GPIO_DIR            0x134/4     // rw  direction: 0 = output, 1 = input
#define GPIO_CLEARDATAOUT	0x190/4     // Write 1 here to set a given bit    
#define GPIO_SETDATAOUT 	0x194/4     // A 1 here clears the corresponding bit
#define GPIO_DATAOUT		0x138/4     // For reading the GPIO registers

#define GPIO_0_OUT shared_mem[0]
#define GPIO_1_OUT shared_mem[1]
#define GPIO_2_OUT shared_mem[2]

 
#define D11   P8_25
uint32_t *gpio3 = (uint32_t *)GPIO3;
uint32_t *gpio2 = (uint32_t *)GPIO2;
uint32_t *gpio1 = (uint32_t *)GPIO1;
uint32_t *gpio0 = (uint32_t *)GPIO0;;
//volatile uint32_t* shared_mem = (volatile uint32_t *) PRU_SHARED_MEM_ADDR;


int main()
{
    
    CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;
	volatile int* shared_mem = (volatile int *) PRU_SHARED_MEM_ADDR;
	// initialize shared memory
	GPIO_0_OUT = 0;  // gpio_0 Data to output from pru
	GPIO_1_OUT = 0;  // gpio_1 Data to output from pru
	GPIO_2_OUT = 0;  // gpio_2 Data to output from pru
	
    gpio0[GPIO_DIR] &= ~D11;

    gpio0[GPIO_CLEARDATAOUT] = P8_25;
        
}
