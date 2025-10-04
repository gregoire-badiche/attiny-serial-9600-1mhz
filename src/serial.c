/** 
 * ATtiny serial (9600 bauds, 1MHz clock speed)
 *  
 * Copyright (C) 2025  Grégoire Badiche
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <avr/io.h>
#include "serial.h"

void serial_init(void)
{
#ifndef DISABLE_TX
    DDRB |= (1 << TX_PIN);
    PORTB |= (1 << TX_PIN);
#endif
#ifndef DISABLE_RX
    DDRB &= ~(1 << RX_PIN);
#endif
}

#ifndef DISABLE_RX

char read()
{
    char c;
    asm volatile (
                                        /* f = 1000000 / 9600 = 104.17 */
                                        /* --------- Waits for activation */
        "L%=wait_loop:"         "\n\t"
        "in r20, %[port]"       "\n\t"
        "bst r20, %[pin_no]"    "\n\t"
        "brts L%=wait_loop"     "\n\t"
                                        /* --------- Waits 103 clock cycle (start bit) */
        "cli"                   "\n\t"  /* 1 -> 1 */
        "ldi r20, 33"           "\n\t"  /* 1 -> 2 */
        "L%=activation:"        "\n\t"  /* 98 -> 100 */
        "dec r20"               "\n\t"  /* 1 */
        "brne L%=activation"    "\n\t"  /* 2 */
        "nop"                   "\n\t"  /* 1 -> 101 */
        "nop"                   "\n\t"  /* 1 -> 102 */
        "ldi r18, 8"            "\n\t"  /* 1 -> 103 */

        "L%=read:"              "\n\t"  /* --------- Main loop (x8)*/
                                        /* --------- Waits 104 clock cycle */
        "ldi r20, 17"           "\n\t"  /* 1 -> 1 */
        "L%=delay1:"            "\n\t"  /* 50 -> 51 */
        "dec r20"               "\n\t"  /* 1 */
        "brne L%=delay1"        "\n\t"  /* 2 */
        "nop"                   "\n\t"  /* 1 -> 52 */
                                        /* --------- Reading of the bit */
        "in r19, %[port]"       "\n\t"  /* 1 -> 53 */
        "bst r19, %[pin_no]"    "\n\t"  /* 1 -> 54 */
        "bld r19, 0"            "\n\t"  /* 1 -> 55 */
        "ror r19"               "\n\t"  /* 1 -> 56 */
        "ror %[c]"              "\n\t"  /* 1 -> 57 */
                                        /* --------- End of bit reading */
        "ldi r20, 14"           "\n\t"  /* 1 -> 58 */
        "L%=delay2:"            "\n\t"  /* 41 -> 99 */
        "dec r20"               "\n\t"  /* 1 */
        "brne L%=delay2"        "\n\t"  /* 2 */
        "nop"                   "\n\t"  /* 1 -> 100 */
        "nop"                   "\n\t"  /* 1 -> 101 */
        "dec r18"               "\n\t"  /* 1 -> 102 */
        "brne L%=read"          "\n\t"  /* 2 -> 104 */

                                        /* --------- Waits for stop bits */
        "L%=end_loop:"          "\n\t"
        "in r20, %[port]"       "\n\t"
        "bst r20, %[pin_no]"    "\n\t"
        "brtc L%=end_loop"      "\n\t"

        "sei"                   "\n\t"  /* Re-enable interrupts */
        : [c] "=r" (c)
        : [pin_no] "I" (RX_PIN),
          [port] "I" (_SFR_IO_ADDR(PINB))
        : "r18", "r19", "r20"
    );
    return c;
}

void recieve(char *buff, int len)
{
    for (int i = 0; i < len; i++)
    {
        *buff = read();

        if (*buff == '\n' || *buff == '\r' || *buff == '\0')
        {
            *buff = '\0';
            return;
        }

        buff++;
    }
}

void recieve_buff(char *buff, int len)
{
    for (int i = 0; i < len; i++)
        *buff++ = read();
}

#endif

#ifndef DISABLE_TX

void write(char c)
{
    asm volatile (
                                        /* f = 1000000 / 9600 = 104.17 */
        "cli"                   "\n\t"  /* Does not count */
        "cbi %[port], %[pin_no]""\n\t"  /* 1 -> (1) */
        "nop"                   "\n\t"  /* 1 -> (2) */
        "nop"                   "\n\t"  /* 1 -> (3) */
        "nop"                   "\n\t"  /* 1 -> (4) */
        "ldi r19, 8"            "\n\t"  /* 1 -> (5) */
        "L%=bit_start:"         "\n\t"
        "nop"                   "\n\t"  /* 1 -> 7 (6)*/
        "nop"                   "\n\t"  /* 1 -> 8 (7) */
        "ldi r18, 31"           "\n\t"  /* 1 -> 9 (8) */
        "L%=activation:"        "\n\t"  /* 92 -> 101 (100) */
        "dec r18"               "\n\t"  /* 1 */
        "brne L%=activation"    "\n\t"  /* 2 */
        "ror %[c]"              "\n\t"  /* 1 -> 102 (101) */
        "brcs L%=bit_high"      "\n\t"     /* 1 (not taken) -> 103 (102) */
        "nop"                   "\n\t"  /* 1 -> 104 (103) */
        "cbi %[port], %[pin_no]""\n\t"  /* 1 -> 1 */
        "rjmp L%=bit_end"       "\n\t"  /* 2 -> 3 */
        "L%=bit_high:"          "\n\t"     /* 2 (taken) -> 104 (103) */
        "sbi %[port], %[pin_no]""\n\t"  /* 1 -> 1 */
        "nop"                   "\n\t"  /* 1 -> 2 */
        "nop"                   "\n\t"  /* 1 -> 3 */
        "L%=bit_end:"           "\n\t"  /* 3 from here */
        "dec r19"               "\n\t"  /* 1 -> 4 */
        "brne L%=bit_start"     "\n\t"  /* 2 -> 6 (5) */
        "ldi r18, 33"           "\n\t"  /* 1 -> (6) */
        "L%=bit_stop:"          "\n\t"  /* 98 -> (104) */
        "dec r18"               "\n\t"  /* 1 */
        "brne L%=bit_stop"      "\n\t"  /* 2 */
        "sbi %[port], %[pin_no]""\n\t"  /* 1 -> 1 */
        "ldi r18, 33"           "\n\t"  /* 1 -> 2 */
        "L%=bit_stop2:"         "\n\t"  /* 101 -> 103 */
        "dec r18"               "\n\t"  /* 1 */
        "brne L%=bit_stop2"     "\n\t"  /* 2 */
        "sei"                   "\n\t"  /* 1 -> 104 */
        ::  [c] "r" (c),
            [pin_no] "I" (TX_PIN),
            [port] "I" (_SFR_IO_ADDR(PORTB))
        :   "r18", "r19"
    );
}

void print(const char* str)
{
    while(*str)
        write(*str++);
}

void println(const char* str)
{
    print(str);
    print("\r\n");
}

#endif
