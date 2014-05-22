#ifndef __ETH_H__
#define __ETH_H__

#define ETH_IPV4 0x0800
#define ETH_ARP 0x0806
#define ETH_IPV6 0x86DD

void eth_write_header(struct buffer* b, const char* src, const char* dst, uint16_t type);
int eth_get_type();
char* eth_get_src();
char* eth_get_dst();
#endif 
