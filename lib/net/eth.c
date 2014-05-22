#include <stdint.h>
#include "buffer.h"
#include "net.h"
#include "ip.h"

void eth_write_header(struct buffer* b, const char* src, const char* dst, uint16_t type) {
	uint8_t t[2];
	t[0] = (type & 0xFF00) >> 8;
	t[1] = (type & 0x00FF);

	buffer_write(b, 6, dst);
	buffer_write(b, 6, src);
	buffer_write(b, 2, &t[0]);
}

char* eth_get_dst() {
	return &net_buffer[0x00];
}

char* eth_get_src() {
	return &net_buffer[0x06];
}

uint16_t eth_get_type(uint16_t type) {
	return htons(*((uint16_t*)&net_buffer[0x0c]));
}

