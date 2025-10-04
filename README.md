# ATtiny serial (9600 bauds, 1MHz clock speed)

A bit-banged serial library for the older ATtiny serie MCU, running at 9600 bauds for a clock frequency of 1MHz.

This library is synchrounous and blocking. It currently supports only `8N1` communication at 9600 bauds, for a clock frequency of 1MHz.

## Features

- Lightweight (288 bytes compiled, 3.5% IRAM, ~64 byte for RX or TX)
- Precise timings with inline assembly
- Choose to activate or deactivate RX/TX for your needs
- Open Source code!

## Usage

```c
// Optional, these are the defaults
#define TX 0
#define RX 1

// If you wish to disable RX or TX
// #define DISABLE_RX
// #define DISABLE_TX

#include <utils/delay.h>
#include "serial.h"

int main(void)
{
    serial_init();

    char c = 0;

    for (;;)
    {
        println("Hello, world!";)
        println("Continue? [y/N]");
        c = read();
        if (c == 'y' || c == 'Y')
            break;
    }

    for (;;);
}
```
