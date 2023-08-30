////////////////////////////////////////////////////////////////////////////////
// BusMonitor_init_pins.c is used to initialize the pins for reading from the
// HI-1575 chip using bus monitor mode.
// 
// Refer to https://vadl.github.io/beagleboneblack/2016/07/29/setting-up-bbb-gpio
// to learn more about the gpio addressing requirements for the Beaglebone Black
//
// NOTE: for any physical gpio pin: GPIO(port)_(bit) = 32*(port) + (bit) 
// when addressed in the file.
// For example: GPIO2_25 = 32*2 + 25 making it gpio89 in the file directory.
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>

FILE* f;
const char* input = "in";   // Used to write "in" to a gpio's direction file.
const char* output = "out"; // Used to write "out" to a gpio's direction file.
const char* high = (char*)1;
const char* low = (char*)0;

void setTimerPin() {
    char command[50];
    snprintf(command, sizeof(command), "cd /dev/pwm/timer7");
    system(command);
    snprintf(command, sizeof(command), "echo 83 > period");
    system(command);
    snprintf(command, sizeof(command), "echo 41 > duty_cycle");
    system(command);
    snprintf(command, sizeof(command), "echo 1 > enable");
    system(command);
}

void makeGpioInput(FILE* filename) {
    // Takes the input "filename" for the gpio pins file location and 
    // writes "in" to the file to configure the pin as an input.
    fseek(filename, 0, SEEK_SET);
    fwrite(input, sizeof(input), 1, filename);
    fflush(filename);
    fclose(filename);
    return;
}

void makeGpioOutput(FILE* filename) {
    // Takes the input "filename" for the gpio pins file location and 
    // writes "in" to the file to configure the pin as an input.
    fseek(filename, 0, SEEK_SET);
    fwrite(output, sizeof(output), 1, filename);
    fflush(filename);
    fclose(filename);
    return;
}

void makeGpioHigh(FILE* filename) {
    fseek(filename, 0, SEEK_SET);
    fwrite(high, sizeof(high), 1, filename);
    // fseek(filename,0,SEEK_SET);
    fprintf(filename, "%d", 1);
    fflush(filename);
    fclose(filename);
    return;
}

void initializePins(void) {
    // Initialize the HI-1575's D0-D15 data pins as inputs for Bus Monitor Mode.
    // These are associated with the ODD NUMBER GPIO pins on the P8 header [3-33]
    //
    // fileDescriptor = fopen("filepathOfGPIO#","r+"); // BBB pin -> HI-1575 pin
    f = fopen("/sys/class/gpio/gpio38/direction", "r+"); // P8.3  -> D0
    makeGpioInput(f);
    f = fopen("/sys/class/gpio/gpio34/direction", "r+"); // P8.5  -> D1
    makeGpioInput(f);
    f = fopen("/sys/class/gpio/gpio66/direction", "r+"); // P8.7  -> D2
    makeGpioInput(f);
    f = fopen("/sys/class/gpio/gpio69/direction", "r+"); // P8.9  -> D3
    makeGpioInput(f);
    f = fopen("/sys/class/gpio/gpio45/direction", "r+"); // P8.11 -> D4
    makeGpioInput(f);
    f = fopen("/sys/class/gpio/gpio23/direction", "r+"); // P8.13 -> D5
    makeGpioInput(f);
    f = fopen("/sys/class/gpio/gpio47/direction", "r+"); // P8.15 -> D6
    makeGpioInput(f);
    f = fopen("/sys/class/gpio/gpio27/direction", "r+"); // P8.17 -> D7
    makeGpioInput(f);
    f = fopen("/sys/class/gpio/gpio22/direction", "r+"); // P8.19 -> D8
    makeGpioInput(f);
    f = fopen("/sys/class/gpio/gpio62/direction", "r+"); // P8.21 -> D9
    makeGpioInput(f);
    f = fopen("/sys/class/gpio/gpio36/direction", "r+"); // P8.23 -> D10
    makeGpioInput(f);
    f = fopen("/sys/class/gpio/gpio32/direction", "r+"); // P8.25 -> D11
    makeGpioInput(f);
    f = fopen("/sys/class/gpio/gpio86/direction", "r+"); // P8.27 -> D12
    makeGpioInput(f);
    f = fopen("/sys/class/gpio/gpio87/direction", "r+"); // P8.29 -> D13
    makeGpioInput(f);
    f = fopen("/sys/class/gpio/gpio10/direction", "r+"); // P8.31 -> D14
    makeGpioInput(f);
    f = fopen("/sys/class/gpio/gpio9/direction", "r+");  // P8.33 -> D15
    makeGpioInput(f);

    // Initialized inputs for the P9 header
    f = fopen("/sys/class/gpio/gpio112/direction", "r+");// P9.30 -> SYNC
    makeGpioInput(f);
    f = fopen("/sys/class/gpio/gpio2/direction", "r+");  // P9.22 -> RCVA
    makeGpioInput(f);

    // Initialize the HI-1575's SAM register pins as outputs for Bus Monitor Mode.
    f = fopen("/sys/class/gpio/gpio4/direction", "r+");  // P9.18 -> MR
    makeGpioOutput(f);
    f = fopen("/sys/class/gpio/gpio14/direction", "r+"); // P9.26 -> R/!W
    makeGpioOutput(f);
    f = fopen("/sys/class/gpio/gpio50/direction", "r+"); // P9.14 -> CLK
    makeGpioOutput(f);
    f = fopen("/sys/class/gpio/gpio113/direction", "r+"); // P9.28 -> STRB
    makeGpioOutput(f);

    setTimerPin();

    return;
}

void initializeALTERNATEPins(void) {
    f = fopen("/sys/class/gpio/gpio45/direction", "r+"); // P8.11  -> D0
    makeGpioInput(f);
    f = fopen("/sys/class/gpio/gpio44/direction", "r+"); // P8.12  -> D1
    makeGpioInput(f);
    f = fopen("/sys/class/gpio/gpio23/direction", "r+"); // P8.13  -> D2
    makeGpioInput(f);
    f = fopen("/sys/class/gpio/gpio26/direction", "r+"); // P8.14  -> D3
    makeGpioInput(f);
    f = fopen("/sys/class/gpio/gpio47/direction", "r+"); // P8.15 -> D4
    makeGpioInput(f);
    f = fopen("/sys/class/gpio/gpio46/direction", "r+"); // P8.16 -> D5
    makeGpioInput(f);
    f = fopen("/sys/class/gpio/gpio27/direction", "r+"); // P8.17 -> D6
    makeGpioInput(f);
    f = fopen("/sys/class/gpio/gpio65/direction", "r+"); // P8.18 -> D7
    makeGpioInput(f);
    f = fopen("/sys/class/gpio/gpio30/direction", "r+"); // P8.11 -> D8
    makeGpioInput(f);
    f = fopen("/sys/class/gpio/gpio60/direction", "r+"); // P8.12 -> D9
    makeGpioInput(f);
    f = fopen("/sys/class/gpio/gpio32/direction", "r+"); // P8.13 -> D10
    makeGpioInput(f);
    f = fopen("/sys/class/gpio/gpio50/direction", "r+"); // P8.14 -> D11
    makeGpioInput(f);
    f = fopen("/sys/class/gpio/gpio48/direction", "r+"); // P8.15 -> D12
    makeGpioInput(f);
    f = fopen("/sys/class/gpio/gpio51/direction", "r+"); // P8.16 -> D13
    makeGpioInput(f);
    f = fopen("/sys/class/gpio/gpio5/direction", "r+"); // P8.17 -> D14
    makeGpioInput(f);
    f = fopen("/sys/class/gpio/gpio4/direction", "r+");  // P8.18 -> D15
    makeGpioInput(f);

    // Initialized inputs for the P9 header
    f = fopen("/sys/class/gpio/gpio112/direction", "r+");// P9.30 -> SYNC
    makeGpioInput(f);
    f = fopen("/sys/class/gpio/gpio2/direction", "r+");  // P9.22 -> RCVA
    makeGpioInput(f);

    // Initialize the HI-1575's SAM register pins as outputs for Bus Monitor Mode.
    f = fopen("/sys/class/gpio/gpio3/direction", "r+");  // P9.18 -> MR
    makeGpioOutput(f);
    f = fopen("/sys/class/gpio/gpio14/direction", "r+"); // P9.26 -> R/!W
    makeGpioOutput(f);
    f = fopen("/sys/class/gpio/gpio49/direction", "r+"); // P9.14 -> CLK
    makeGpioOutput(f);
    f = fopen("/sys/class/gpio/gpio113/direction", "r+"); // P9.28 -> STRB
    makeGpioOutput(f);

    setTimerPin();

    return;
}
