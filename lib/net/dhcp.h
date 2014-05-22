#ifndef __DHCP_H__
#define __DHCP_H__

//#define DHCP_MAGIC 0x63 82 53 63

#define DHCP_MAGIC 0x63538263
#define DHCP_OPT_TYPE 53
#define DHCP_DISCOVER 1
#define DHCP_OFFER 2
#define DHCP_REQUEST 3
#define DHCP_DECLINE 4
#define DHCP_ACK 5
#define DHCP_NAK 6
#define DHCP_RELEASE 7
#define DHCP_INFORM 8

#define DHCP_OPT_END 255
#define DHCP_OPT_REQUEST 50

int dhcp_is_magic();
uint32_t ghcp_get_yiaddr();
uint32_t ghcp_get_siaddr();
uint32_t dhcp_get_opt(uint8_t opt);

#endif 
