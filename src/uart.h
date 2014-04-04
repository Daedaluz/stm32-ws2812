#ifndef __UART_H__
#define __UART_H__

void uart1_init();
void cputc(uint8_t);
uint16_t cgetc();
void cputs(const char*);

#endif /* __UART_H__ */

