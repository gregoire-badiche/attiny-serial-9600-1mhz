#define F_CPU 1000000UL

#include <avr/io.h>
#include <string.h>

// Note : place these defines BEFORE the include
#define DISABLE_RX

#include "../../src/serial.h"

int main(void)
{
    serial_init();

    println("The pin 1 is free to use!\r\n");

    for (;;);

    return 0;
}
