#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define charOffset 48   // This is used because when returning a value from the 
                        // file path "/sys/class/gpio/gpio%d/value", it is returned
                        // as a char value. The ASCII for '0' is 48 and the 
                        // ASCII value for '1' is 49.
#define D0  38
#define D1  34
#define D2  66
#define D3  69
#define D4  45
#define D5  23
#define D6  47
#define D7  27
#define D8  22
#define D9  62
#define D10 36
#define D11 32
#define D12 86
#define D13 87
#define D14 10
#define D15 9


#define MR    3
#define SYNC  112
#define RCVA  2
#define R_W   14
#define CLK   49
#define STRB  113


int read_gpio(int pin) {
    char path[50];
    uint8_t value_fd, value;
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", pin);

    if ((value_fd = open(path, O_RDONLY)) < 0) {
        perror("open failed");
        exit(EXIT_FAILURE);
    }
    if (read(value_fd, &value, sizeof(value)) < 0) {
        perror("read failed");
        exit(EXIT_FAILURE);
    }
    close(value_fd);

    return value - charOffset;
}

char* GPIO_PATH = "/sys/class/gpio/";
#define MAX_BUF 64

typedef enum {
    LOW,
    HIGH
} gpio_value_t;

int gpio_write(int pin, gpio_value_t value) {
    char buf[MAX_BUF];
    int fd;
    ssize_t bytes_written;

    // Export the pin
    snprintf(buf, sizeof(buf), "/sys/class/gpio/export");
    fd = open(buf, O_WRONLY);
    if (fd == -1) {
        perror("gpio_write: Failed to open export for writing");
        return -1;
    }
    snprintf(buf, sizeof(buf), "%d", pin);
    bytes_written = write(fd, buf, strlen(buf));
    close(fd);
    if (bytes_written == -1) {
        perror("gpio_write: Failed to write to export");
        return -1;
    }

    // Set the direction to "out"
    snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio%d/direction", pin);
    fd = open(buf, O_WRONLY);
    if (fd == -1) {
        perror("gpio_write: Failed to open direction for writing");
        return -1;
    }
    bytes_written = write(fd, "out", 3);
    close(fd);
    if (bytes_written == -1) {
        perror("gpio_write: Failed to write to direction");
        return -1;
    }

    // Write the value to the pin
    snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio%d/value", pin);
    fd = open(buf, O_WRONLY);
    if (fd == -1) {
        perror("gpio_write: Failed to open value for writing");
        return -1;
    }
    bytes_written = write(fd, value == HIGH ? "1" : "0", 1);
    close(fd);
    if (bytes_written == -1) {
        perror("gpio_write: Failed to write to value");
        return -1;
    }

    return 0;
}


// void writegpio(int pin, int value)
// {
//     char path[50];
//     uint8_t value_fd;
//     printf("%s\n", path);
//     snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", pin);

//     if((value_fd = open(path, O_WRONLY)) < 0) {
//         perror("open failed");
//         exit(EXIT_FAILURE);
//     }
//     if(write(value_fd, &value, sizeof(value)) < 0) {
//         printf("value: %x\n",&value);
//         perror("write failed");
//         exit(EXIT_FAILURE);
//     }
//     close(value_fd);
// }

void pulse_mr() {
    // printf("MR pin pulse\n");
    return;
    // gpio_write(MR, HIGH);
    // sleep(0.004);;
    // gpio_write(MR, LOW);
    // sleep(0.004);
    // return;
}

// rw_high

// reg_low

// strb_low //set RW = high, and REG = low

void pulse_strb() {
    //  Use this function to set the 
    // printf("RW = HIGH, REG = LOW, STRB = pulsed\n");
    sleep(0.05);
    return;
    // writegpio(RW, HIGH);
    // writegpio(REG, LOW);
    // gpio_write(STRB, LOW);
    // sleep(0.004);
    // gpio_write(STRB, HIGH);
    // sleep(0.004);
    // return;
}




