#include <stdint.h>
#include "net.h"
#include "buffer.h"


uint8_t mac_bcast[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t mac_addr[6] = {0x54, 0x55, 0x58, 0x10, 0x00, 0x01};
uint8_t ip_addr[4] = {0, 0, 0, 0};

uint8_t net_buffer[NET_BUFFER_SIZE];
struct buffer netwbuff1;
struct buffer netwbuff2;

uint32_t net_nrecv;

void net_init(const char* mac){
	buffer_init(&netwbuff1, 500, &net_buffer[0]);
	buffer_init(&netwbuff2, 500, &net_buffer[1000]);
	int x = 0;
	for(x = 0; x < 6; x++)
		mac_addr[x] = mac[x];
}

void net_send() {
	enc28j60PacketSend(buffer_len(&netwbuff1), netwbuff1.start);
}

uint32_t net_recv() {
	net_nrecv = enc28j60PacketReceive(1500, &net_buffer);
	return net_nrecv;
}

void net_set_ip(uint32_t ip) {
	char* d = (char*)&ip;
	int i;
	for(i = 0; i < 4; i++)
		ip_addr[i] = d[i];
}

