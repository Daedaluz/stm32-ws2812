#ifndef __ARP_H__
#define __ARP_H__

#include <stdint.h>
#include "buffer.h"
void arp_gratuitous(struct buffer* b);
void arp_reply(struct buffer* b, char* mac, uint32_t to);
char* arp_get_sender_mac();
uint32_t arp_get_sender_ip();

#endif 
