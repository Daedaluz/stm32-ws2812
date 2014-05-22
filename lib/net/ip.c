#include <stdint.h>
#include "buffer.h"
#include "ip.h"
#include "net.h"

uint16_t checksum(uint8_t *buf, uint16_t len) 
{
	uint32_t sum = 0;
	while(len >1) {
		sum += 0xFFFF & (*buf<<8|*(buf+1));
		buf+=2;
		len-=2;
	}
	if (len)
		sum += (0xFF & *buf)<<8;
	while (sum>>16)
		sum = (sum & 0xFFFF)+(sum >> 16);
	return( (uint16_t) sum ^ 0xFFFF);
}

uint16_t htons(uint16_t c) {
	return (((c & 0x00FF) << 8) | ((c & 0xFF00) >> 8));
}

uint32_t htonl(uint32_t c) {
	return (
			((c & 0xFF000000) >> 24)|
			((c & 0x00FF0000) >> 8)|
			((c & 0x0000FF00) << 8)|
			((c & 0x000000FF) << 24)
	);
}

void ip_write_header(struct buffer* b, uint8_t proto,  uint32_t src, uint32_t dst, uint16_t length) {
	struct ip_header* hdr = (struct ip_header*)b->cursor;
	buffer_write(b, sizeof(struct ip_header), (void*)hdr);
	hdr->version_length = 0x45;
	hdr->dscp_ecn = 0;
	hdr->length = length + 20;
	hdr->identification = 0;
	hdr->flags_fragment = 0x4000;
	hdr->ttl = 64;
	hdr->protocol = proto;
	hdr->crc = 0;
	hdr->src = src;
	hdr->dst = dst;
	hdr->length = htons(hdr->length);
	hdr->flags_fragment = htons(hdr->flags_fragment);
	hdr->crc = checksum((void*)hdr, 20);
	hdr->crc = htons(hdr->crc);
}


void ip_write_udp_message(struct buffer* b, uint32_t src, uint32_t dst, uint16_t srcp, 
							uint16_t dstp, uint16_t length, char* data) {
	ip_write_header(b, IP_UDP, src, dst, length + 8);
	uint16_t _srcp = htons(srcp);
	uint16_t _dstp = htons(dstp);
	uint16_t _length = htons(length + 8);
	uint16_t crc = 0;
	buffer_write(b, 2, (void*)&_srcp);
	buffer_write(b, 2, (void*)&_dstp);
	buffer_write(b, 2, (void*)&_length);
	buffer_write(b, 2, (void*)&crc);
	buffer_write(b, length, data);
}

uint8_t ip_get_protocol() {
	return (uint8_t)net_buffer[0x17];
}

uint16_t ip_udp_get_src() {
	return htons(*(uint16_t*)&net_buffer[0x22]);
}

uint16_t ip_udp_get_dst() {
	return htons(*(uint16_t*)&net_buffer[0x24]);
}

uint16_t ip_udp_get_len() {
	return (htons(*(uint16_t*)&net_buffer[0x26]) - (uint16_t)8);
}

char* ip_udp_get_datap() {
	return &net_buffer[0x2A];
}

uint32_t ip_get_src_addr() {
	return htonl(*(uint32_t*)&net_buffer[0x1A]);
}

uint32_t ip_get_dst_addr() {
	return htonl(*(uint32_t*)&net_buffer[0x1E]);
}


