//#include <stdio.h>
#include <stdint.h>
#include "buffer.h"

void buffer_init(struct buffer* b, uint32_t size, char* p) {
	b->start = p;
	b->cursor = p;
	b->size = size;
}

void buffer_reset(struct buffer* b) {
	b->cursor = b->start;
	memset(b->start, 0, b->size);
}

uint32_t buffer_write(struct buffer* b, uint32_t size, const char* p) {
	uint32_t c;
	for(c = 0;b->size > buffer_len(b) && c < size; c++, b->cursor++) {
		b->cursor[0] = p[c];
	}
	return c;
}

uint32_t buffer_remains(struct buffer* b) {
	uint32_t tmp = buffer_len(b);
	return b->size - tmp;
}

void buffer_flush(struct buffer* b) {
	uint32_t c = 0; 
	for(c = 0; c < b->size; c++)
		b->start[c] = 0;
	b->cursor = b->start;
}

uint32_t buffer_len(struct buffer* b) {
	return (uint32_t)(b->cursor - b->start);
}

int buffer_skip(struct buffer* b, uint32_t size) {
	b->cursor += size;
}

int buffer_zero(struct buffer* b, uint32_t size) {
	uint32_t c;
	for(c = 0;b->size > buffer_len(b) && c < size; c++, b->cursor++) {
		b->cursor[0] = 0;
	}
	return c;
}

void buffer_write_uint8(struct buffer* b, uint8_t c) {
	buffer_write(b, 1, &c);
}

void buffer_write_uint16(struct buffer* b, uint16_t c) {
	char* d = (char*)&c;
	buffer_write_uint8(b, d[1]);
	buffer_write_uint8(b, d[0]);
}

void buffer_write_uint32(struct buffer* b, uint32_t c) {
	char* d = (char*)&c;
	buffer_write_uint8(b, d[3]);
	buffer_write_uint8(b, d[2]);
	buffer_write_uint8(b, d[1]);
	buffer_write_uint8(b, d[0]);
}

