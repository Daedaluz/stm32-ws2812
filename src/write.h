#ifndef __WRITE_H__
#define __WRITE_H__

#include <stdint.h>
#define BUFFER_SIZE (0x3840 + 42)
//#define BUFFER_SIZE (0x1840 + 42)
//#define BUFFER_SIZE (6*24 + 42)

extern uint16_t write_buffer[BUFFER_SIZE];

void write_dmx(uint32_t offset, uint32_t size, char* data);
void write_ws2812(uint32_t offset, uint32_t size, char* data);
void write_ws2801(uint32_t offset, uint32_t size, char* data);

#endif /* __WRITE_H__ */

