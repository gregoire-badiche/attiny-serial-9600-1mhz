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

#ifndef _SERIAL_H_
#define _SERIAL_H_

void serial_init(void);

#ifndef DISABLE_TX

#ifndef TX_PIN
#define TX_PIN 0
#endif

void write(char c);
void print(const char* str);
void println(const char* str);

#endif

#ifndef DISABLE_RX

#ifndef RX_PIN
#define RX_PIN 1
#endif

char read();
void recieve(char *buff, int len);

#endif


#endif