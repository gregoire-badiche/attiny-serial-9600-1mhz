#define F_CPU 1000000UL

#include <avr/io.h>
#include <string.h>
#include "serial.h"

char input[32];

int main(void)
{
    serial_init();

    for (;;)
    {
        println("What is your name?");

        recieve(input, 32);

        print("Hello ");
        print(input);
        println("!");
    }

    return 0;
}
