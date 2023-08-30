////////////////////////////////////////
//	1575_functions.pru0.c
//	All functions used to read/write the HI-1575 chip gpio pins
// This code tests the RCVA pin and the Sync pin using a LED on a breadboard 
// connected to pin P8_39. This test can be used with any pin not being utilized by the cape
////////////////////////////////////////
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <pru_cfg.h>
#include "resource_table_empty.h"
#include "cape_test_gpio.h"
#define GPIO_0_OUT shared_mem[0]
#define GPIO_1_OUT shared_mem[1]
#define GPIO_2_OUT shared_mem[2]
//  Input Pin
int read_RCVA()
{
    // set to input and then read the pin
	gpio0[GPIO_DIR] |= RCVA;
	return ((gpio0[GPIO_DATAOUT] >> 2) & 1);
}

// Output Pin
void write_REG(int high)
{
	// set pin to output
	gpio0[GPIO_DIR] &= ~REG;
    // if a value is put into int high, set to 1
	if (high) gpio0[GPIO_SETDATAOUT] |= P9_24;
    // else, set to 0
	else gpio0[GPIO_CLEARDATAOUT] |= P9_24;
}

// Output Pin
void write_RW(int high)
{
	 // set pin to output
	gpio0[GPIO_DIR] &= ~RW;
    // if a value is put into int high, set to 1
	if (high) gpio0[GPIO_SETDATAOUT] |= P9_26;
    // else, set to 0
	else gpio0[GPIO_CLEARDATAOUT] |= P9_26;
}

// Output Pin
void write_STRB(int high)
{
	 // set pin to output
	gpio3[GPIO_DIR] &= ~STRB;
    // if a value is put into int high, set to 1
	if (high) gpio3[GPIO_SETDATAOUT] |= STRB;
    // else, set to 0
	else gpio3[GPIO_CLEARDATAOUT] |= STRB;
}
// Output Pin
void write_MR(int high)
{
	// set pin to output
	gpio0[GPIO_DIR] &= ~MR;
    // if a value is put into int high, set to 1
	if (high) gpio0[GPIO_SETDATAOUT] |= MR; // Set MR High
    // else, set to 0
	else gpio0[GPIO_CLEARDATAOUT] |= MR; // Set MR Low
}


// Bidirectional Pin
void write_sync(int high)  //gpio3[16]
{
    // set pin to output
	gpio3[GPIO_DIR] &= ~SYNC;
    // if a value is put into int high, set to 1
	if (high) gpio3[GPIO_SETDATAOUT] = SYNC;
    // else, set to 0
	else gpio3[GPIO_CLEARDATAOUT] = SYNC;
}

//input pin
int read_sync()
{ 
    // set the pin to input
	gpio3[GPIO_DIR] |= SYNC;
    // read the pin and return
	return ((gpio3[GPIO_DATAOUT] >> 16) & 1);
}

// bidirectional pins D0 - D15
// reads these pins
int read_data_bus()
{	
//	char data_vals[4];
	// setting D0 - D15 pins to inputs
	gpio0[GPIO_DIR] |= (D15 | D11 | D5 | D7 | D8);
	gpio1[GPIO_DIR] |= (D4 | D6 | D9 | D10 | D11 | D0 | D1);
	gpio2[GPIO_DIR] |= (D12 | D13 | D2 | D3);
	
	// data_bus_values holds all of the bit values from D0 - D15
	int data_bus_values = 0;
    // reads D15; shift 15 bits, and then read gpio0[9]
	data_bus_values |= (((gpio0[GPIO_DATAOUT] >> 9 ) & 1) << 15);
    // reads D14; shift 14 bits, and then read gpio0[10]
	data_bus_values |= (((gpio0[GPIO_DATAOUT] >> 10 ) & 1) << 14);
    // reads D13; shift 13 bits, and then read gpio2[23]
	data_bus_values |= (((gpio2[GPIO_DATAOUT] >> 23 ) & 1) << 13);    
    // reads D12; shift 12 bits, and then read gpio2[22]
	data_bus_values |= (((gpio2[GPIO_DATAOUT] >> 22) & 1) << 12); 
    // reads D11; shift 11 bits, and then read gpio1[0]
	data_bus_values |= (((gpio1[GPIO_DATAOUT] >> 0) & 1) << 11);
    // reads D10; shift 10 bits, and then read gpio1[4]
	data_bus_values |= (((gpio1[GPIO_DATAOUT] >> 4) & 1) << 10);
    // reads D9; shift 9 bits, and then read gpio1[30]
	data_bus_values |= (((gpio1[GPIO_DATAOUT] >> 30) & 1) << 9 );
    // reads D8; shift 8 bits, and then read gpio0[22]
	data_bus_values |= (((gpio0[GPIO_DATAOUT] >> 22) & 1) << 8 ); 
    // reads D7; shift 7 bits, and then read gpio0[27]
	data_bus_values |= (((gpio0[GPIO_DATAOUT] >> 27) & 1) << 7);
    // reads D6; shift 6 bits, and then read gpio1[15]
	data_bus_values |= (((gpio1[GPIO_DATAOUT] >> 15) & 1) << 6);
    // reads D5; shift 5 bits, and then read gpio0[23]
	data_bus_values |= (((gpio0[GPIO_DATAOUT] >> 23 ) & 1) << 5 );
    // reads D4; shift 4 bits, and then read gpio1[13]
	data_bus_values |= (((gpio1[GPIO_DATAOUT] >> 13) & 1) << 4 );
    // reads D3; shift 3 bits, and then read gpio2[5]
	data_bus_values |= (((gpio2[GPIO_DATAOUT] >> 5 ) & 1) << 3 );
    // reads D2; shift 2 bits, and then read gpio2[2]
	data_bus_values |= (((gpio2[GPIO_DATAOUT] >> 2) & 1) << 2 );
    // reads D1; shift 1 bits, and then read gpio1[2]
	data_bus_values |= (((gpio1[GPIO_DATAOUT] >> 2) & 1) << 1); 
    // reads D0; shift 0 bits, and then read gpio1[6]
	data_bus_values |= (((gpio1[GPIO_DATAOUT] >> 6 ) & 1) << 0); 
	
    // return data_bus_values, which contains D0 - D15 bit values
	return data_bus_values;
}
void write_data_bus(int data_bus_values)
{
	// set the pins D0 - D15 to outputs
	gpio0[GPIO_DIR] &= ~(D15  | D11 | D5 | D7 | D8);
	gpio1[GPIO_DIR] &= ~(D4 | D6 | D9 | D10 | D11 | D0 | D1);
	gpio2[GPIO_DIR] &= ~(D12 | D13 | D2 | D3);
	
	// if data_bus_values bit 15 is high, set D15 high
	if ((data_bus_values >> 15) & 1) gpio0[GPIO_SETDATAOUT] = D15;
    // else, set D15 low
	else gpio0[GPIO_CLEARDATAOUT] = D15;
	
    // if data_bus_values bit 14 is high, set D14 high
	if ((data_bus_values >> 14) & 1) gpio0[GPIO_SETDATAOUT] = D14;
    // else, set D14 low
	else gpio0[GPIO_CLEARDATAOUT] = D14;
	
    // if data_bus_values bit 13 is high, set D13 high
	 if ((data_bus_values >> 13) & 1) gpio2[GPIO_SETDATAOUT] = D13;
     // else, set D13 low
	 else gpio2[GPIO_CLEARDATAOUT] = D13;

    // if data_bus_values bit 12 is high, set D12 high
	if ((data_bus_values >> 12) & 1) gpio2[GPIO_SETDATAOUT] = D12;
    // else, set D12 low
	else gpio2[GPIO_CLEARDATAOUT] = D12;
	
    // if data_bus_values bit 11 is high, set D11 high
	if ((data_bus_values >> 11) & 1) gpio1[GPIO_SETDATAOUT] = D11;
    // else, set D11 low
	else gpio1[GPIO_CLEARDATAOUT] = D11;
	
    // if data_bus_values bit 10 is high, set D10 high
	if ((data_bus_values >> 10) & 1) gpio1[GPIO_SETDATAOUT] = D10;
    // else, set D10 low
	else gpio1[GPIO_CLEARDATAOUT] = D10;
	
    // if data_bus_values bit 9 is high, set D9 high
	if ((data_bus_values >> 9) & 1) gpio1[GPIO_SETDATAOUT] = D9;
    // else, set D9 low
	else gpio1[GPIO_CLEARDATAOUT] = D9;
	
    // if data_bus_values bit 8 is high, set D8 high
	if ((data_bus_values >> 8) & 1) gpio0[GPIO_SETDATAOUT] = D8;
    // else, set D8 low
	else gpio0[GPIO_CLEARDATAOUT] = D8;
	
    // if data_bus_values bit 7 is high, set D7 high
	if ((data_bus_values >> 7) & 1) gpio0[GPIO_SETDATAOUT] = D7;
    // else, set D7 low
	else gpio0[GPIO_CLEARDATAOUT] = D7;
	
    // if data_bus_values bit 6 is high, set D6 high
	if ((data_bus_values >> 6) & 1) gpio1[GPIO_SETDATAOUT] = D6;
    // else, set D6 low
	else gpio1[GPIO_CLEARDATAOUT] = D6;
	
    // if data_bus_values bit 5 is high, set D5 high
	if ((data_bus_values >> 5) & 1) gpio0[GPIO_SETDATAOUT] = D5;
    // else, set D5 low
	else gpio0[GPIO_CLEARDATAOUT] = D5;
	
    // if data_bus_values bit 4 is high, set D4 high
	if ((data_bus_values >> 4) & 1) gpio1[GPIO_SETDATAOUT] = D4;
    // else, set D4 low
	else gpio1[GPIO_CLEARDATAOUT] = D4;
	
    // if data_bus_values bit 3 is high, set D3 high
	if ((data_bus_values >> 3) & 1) gpio2[GPIO_SETDATAOUT] = D3;
    // else, set D3 low
	else gpio2[GPIO_CLEARDATAOUT] = D3;
	
    // if data_bus_values bit 2 is high, set D2 high
	if ((data_bus_values >> 2) & 1) gpio2[GPIO_SETDATAOUT] = D2;
    // else, set D2 low
	else gpio2[GPIO_CLEARDATAOUT] = D2;
	
    // if data_bus_values bit 1 is high, set D1 high
	if ((data_bus_values >> 1) & 1) gpio1[GPIO_SETDATAOUT] = D1;
    // else, set D1 low
	else gpio1[GPIO_CLEARDATAOUT] = D1;
	
    // if data_bus_values bit 0 is high, set D0 high
	if ((data_bus_values >> 0) & 1) gpio1[GPIO_SETDATAOUT] = D0;
    // else, set D0 low
	else gpio1[GPIO_CLEARDATAOUT] = D0;
}

// Output Pin 
void writeCHBCHA() // gpio1[28]
{
	 // set pin to output
	gpio1[GPIO_DIR] &= ~CHACHB;
    // set CHBCHA low
	gpio1[GPIO_CLEARDATAOUT] |= CHACHB;
}


// Input Pin
// read the error pin
int readERROR() // P9_16 gpio1[19]
{
    // set the pin to input
	gpio1[GPIO_DIR] |= ERROR;
    // read that pin and return
	return (gpio1[GPIO_DATAOUT] >> 19) & 1;
}

//set MR high, wait a millisecond, set MR low, wait a millisecond
void pulse_MR()
{
    // set MR high
	write_MR(1);
	__delay_cycles(millisecond/1000);	  // Wait a millisecond
    // set MR low
	write_MR(0);
	__delay_cycles(millisecond/1000); 	  // Wait a millisecond
}

// read the data bus pins
int read_SAM()
{
    // set STRB low
	write_STRB(0);
    // set RW high
	write_RW(1);
    // set REG low
	write_REG(0);
    // wait
	__delay_cycles(second/1000000); // Wait a millisecond
	return read_data_bus();
}

// write to the SAM 
void write_SAM()
{
    // STRB low
	write_STRB(0);
    // RW low
	write_RW(0);
    // REG high
	write_REG(1);
    //wait
	__delay_cycles(second/1000000); // Wait a millisecond
    // set STRB high
	write_STRB(1);
}
// Pulse STRB low while R/W is low and REG is low
void signal_send()
{
    // set STRB low
	write_STRB(0);
    // set RW low
	write_RW(0);
    // set REG low
	write_REG(0);
    // wait
	__delay_cycles(millisecond); // Wait a millisecond
    // set STRB high
	write_STRB(1);
}

// Read SAM and wait for bit 15 to go to 0
// waiting for transmit to be detected
void wait_for_transmit()
{
	while (((read_SAM() >> 15) & 1) == 1)
	{
		// Wait here
	}
}

void main(void) {
	/* Clear SYSCFG[STANDBY_INIT] to enable OCP master port */
	CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;
	volatile int* shared_mem = (volatile int *) PRU_SHARED_MEM_ADDR;
	// initialize shared memory
	GPIO_0_OUT = 0;  // gpio_0 Data to output from pru
	GPIO_1_OUT = 0;  // gpio_1 Data to output from pru
	GPIO_2_OUT = 0;  // gpio_2 Data to output from pru
	gpio2[GPIO_DIR] &= ~P8_39; // led control
	int count = 0;

	
	
	// testing receiving 1553 messages
	    writeCHBCHA();
	    write_STRB(1);
		pulse_MR(); 
	    count = read_SAM();
		write_SAM();
		while(1){
			if(read_RCVA() == 1){
			
			
				gpio2[GPIO_SETDATAOUT] |= P8_39;
				__delay_cycles(second);		// Wait a second
					gpio2[GPIO_CLEARDATAOUT] |= P8_39;
						__delay_cycles(second);
			// this is where the word would be read
			write_STRB(0);
	        write_RW(1);
	        write_REG(0);
          	__delay_cycles(millisecond/1000); // Wait a microsecond
          	
          	
          	int message = read_data_bus();
          	// try to read data pins
          	if(message > 1){
          		gpio2[GPIO_SETDATAOUT] |= P8_43;
				__delay_cycles(second);	// wait second
			    gpio2[GPIO_CLEARDATAOUT] |= P8_43;
				__delay_cycles(second);
          	}
          	
            write_STRB(1);
            
            // set up an LED on breadboard connected to P8.39
            if(read_sync() == 0){
             	gpio2[GPIO_SETDATAOUT] |= P8_39;
				__delay_cycles(second);		// Wait second
			    gpio2[GPIO_CLEARDATAOUT] |= P8_39;
				__delay_cycles(second);
             }
			
			}
			else{
					gpio2[GPIO_CLEARDATAOUT] |= P8_39;
					__delay_cycles(second);
			}
		}


}
// setting pins to output/input and setting the pins as pinmux 
// see /sys/class/gpio in the terminal for more details
// see /sys/devices/platform/opc/ in the terminal for more details
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
	"/sys/class/gpio/gpio38/direction\0in\0" \
	"/sys/class/gpio/gpio2/direction\0in\0" \
	"/sys/class/gpio/gpio112/direction\0in\0" \
	"/sys/devices/platform/ocp/ocp:P8_03_pinmux/state\0gpio\0" \
	"/sys/devices/platform/ocp/ocp:P8_05_pinmux/state\0gpio\0" \
	"/sys/devices/platform/ocp/ocp:P8_07_pinmux/state\0gpio\0" \
	"/sys/devices/platform/ocp/ocp:P8_09_pinmux/state\0gpio\0" \
	"/sys/devices/platform/ocp/ocp:P8_11_pinmux/state\0gpio\0" \
	"/sys/devices/platform/ocp/ocp:P8_13_pinmux/state\0gpio\0" \
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
	"/sys/devices/platform/ocp/ocp:P9_12_pinmux/state\0gpio\0" \
	"/sys/devices/platform/ocp/ocp:P9_16_pinmux/state\0gpio\0" \
	"/sys/devices/platform/ocp/ocp:P9_18_pinmux/state\0gpio\0" \
	"/sys/devices/platform/ocp/ocp:P9_22_pinmux/state\0gpio\0" \
	"/sys/devices/platform/ocp/ocp:P9_24_pinmux/state\0gpio\0" \
	"/sys/devices/platform/ocp/ocp:P9_26_pinmux/state\0gpio\0" \
	"/sys/devices/platform/ocp/ocp:P9_28_pinmux/state\0gpio\0" \
	"/sys/devices/platform/ocp/ocp:P9_30_pinmux/state\0gpio\0" \
	"/sys/devices/platform/ocp/ocp:P8_39_pinmux/state\0gpio\0" \
	
	"\0\0";
	
