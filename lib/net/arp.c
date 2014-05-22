#include <stdint.h>
#include "net.h"
#include "eth.h"
#include "arp.h"
//#include "buffer.h"

void arp_gratuitous(struct buffer* b) {
	buffer_flush(b);
	eth_write_header(b, mac_addr, mac_bcast, ETH_ARP);
	buffer_write_uint16(b, 1);
	buffer_write_uint16(b, ETH_IPV4);
	buffer_write_uint8(b, 6);
	buffer_write_uint8(b, 4);
	buffer_write_uint16(b, 2);
	buffer_write(b, 6, mac_addr);
	buffer_write(b, 4, ip_addr);
	buffer_write(b, 6, mac_addr);
	buffer_write(b, 4, ip_addr);
}

void arp_reply(struct buffer* b, char* mac, uint32_t to) {
	buffer_flush(b);
	eth_write_header(b, mac_addr, mac, ETH_ARP);
	buffer_write_uint16(b, 1);
	buffer_write_uint16(b, ETH_IPV4);
	buffer_write_uint8(b, 6);
	buffer_write_uint8(b, 4);
	buffer_write_uint16(b, 2);
	

	buffer_write(b, 6, mac_addr);
	buffer_write(b, 4, ip_addr);
	
	buffer_write(b, 6, mac);
	buffer_write_uint32(b, to);
}

char* arp_get_sender_mac() {
	return net_buffer[0x16];
}

uint32_t arp_get_sender_ip() {
	return htonl(*(uint32_t*)&net_buffer[0x1C]);
}

char* arp_get_target_mac() {
	return &net_buffer[0x20];
}

uint32_t arp_get_target_ip() {
	return htonl(*(uint32_t*)&net_buffer[0x26]);
}

