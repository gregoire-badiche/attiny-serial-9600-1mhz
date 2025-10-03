#ifndef _SERIAL_H_
#define _SERIAL_H_

void serial_init(void);

#ifndef DISABLE_TX

#ifndef TX_PIN
#define TX_PIN 0
#endif

char write(char c);
void print(const char* str);

#endif

#ifndef DISABLE_RX

#ifndef RX_PIN
#define RX_PIN 1
#endif

char read();
void recieve(volatile char *buff, int len);

#endif


#endif