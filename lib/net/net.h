#ifndef __NET_H__
#define __NET_H__
#include <stdint.h>

#define NET_BUFFER_SIZE 1512

extern uint8_t mac_bcast[6];
extern uint8_t mac_addr[6];
extern uint8_t ip_addr[4];
extern uint8_t net_buffer[NET_BUFFER_SIZE];
extern struct buffer netwbuff1;
extern struct buffer netwbuff2;
extern uint32_t net_nrecv;

void net_init(const char* mac);
void net_send();
void net_recv();
void net_set_ip(uint32_t ip);


#endif
