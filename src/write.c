#include <stdint.h>
#include <stm32f10x_dma.h>
#include <stm32f10x_tim.h>
#include <stm32f10x_gpio.h>
#include "write.h"

// 10 Meters of ws2812
// also fits dmx (of 513)
// and ws2801 ;)

uint16_t write_buffer[BUFFER_SIZE];

//#define CMPH 17
//#define CMPL 9

#define CMPH 8
#define CMPL 2
void write_ws2812(uint32_t offset, uint32_t size, char* data) {
	uint32_t x = 0;
	for(x = 0; x < size; x++) {
		write_buffer[((x+offset)*8)+0] = (data[x] & 0x80) ? CMPH:CMPL;
		write_buffer[((x+offset)*8)+1] = (data[x] & 0x40) ? CMPH:CMPL;
		write_buffer[((x+offset)*8)+2] = (data[x] & 0x20) ? CMPH:CMPL;
		write_buffer[((x+offset)*8)+3] = (data[x] & 0x10) ? CMPH:CMPL;
		write_buffer[((x+offset)*8)+4] = (data[x] & 0x08) ? CMPH:CMPL;
		write_buffer[((x+offset)*8)+5] = (data[x] & 0x04) ? CMPH:CMPL;
		write_buffer[((x+offset)*8)+6] = (data[x] & 0x02) ? CMPH:CMPL;
		write_buffer[((x+offset)*8)+7] = (data[x] & 0x01) ? CMPH:CMPL;
	}
}

void write_dmx(uint32_t offset, uint32_t size, char* data) {
}

void write_ws2801(uint32_t offset, uint32_t size, char* data) {
}

