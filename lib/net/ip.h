#ifndef __IP_H__
#define __IP_H__
#include "buffer.h"

struct ip_header {
	uint8_t version_length;
	uint8_t dscp_ecn;
	uint16_t length;
	uint16_t identification;
	uint16_t flags_fragment;
	uint8_t ttl;
	uint8_t protocol;
	uint16_t crc;
	uint32_t src;
	uint32_t dst;
};

#define IP_UDP 0x11
#define IP_ICMP 0x01


uint16_t htons(uint16_t c);
void ip_write_udp_message(struct buffer* b, uint32_t src, uint32_t dst, uint16_t srcp, 
							uint16_t dstp, uint16_t length, char* data);


uint32_t ip_is_protocol(uint8_t proto);

uint16_t ip_udp_get_src();
uint16_t ip_udp_get_dst();
uint16_t ip_udp_get_len();
char* ip_udp_get_datap();

uint32_t ip_get_src_addr();
uint32_t ip_get_dst_addr();

#endif 
