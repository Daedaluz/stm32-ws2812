#ifndef __SPI_H__
#define __SPI_H__
#include <stdint.h>

void ENC28J60_CSH(void);
void ENC28J60_CSL(void);
void SPI1_Init(void);
uint8_t	SPI1_ReadWrite(uint8_t writedat);

#endif
