#ifndef __BUFFER_H__
#define __BUFFER_H__
#include <stdint.h>

struct buffer {
	char* start;
	char* cursor;
	uint32_t size;
};

void buffer_init(struct buffer* b, uint32_t size, char* p);
uint32_t buffer_write(struct buffer* b, uint32_t size, const char* p);
void buffer_flush(struct buffer* b);
uint32_t buffer_len(struct buffer* b);

int buffer_skip(struct buffer* b, uint32_t size);
int buffer_zero(struct buffer* b, uint32_t size);

void buffer_write_uint8(struct buffer* b, uint8_t c);
void buffer_write_uint16(struct buffer* b, uint16_t c);
void buffer_write_uint32(struct buffer* b, uint32_t c);

#endif 
