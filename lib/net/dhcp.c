#include <stdint.h>
#include "net.h"
#include "dhcp.h"
#include "ip.h"
#include "eth.h"
#include "buffer.h"

static void dhcp_bootp_init(struct buffer* b, uint32_t siaddr) {
	buffer_flush(&netwbuff2);
	buffer_write_uint8(&netwbuff2, 1);
	buffer_write_uint8(&netwbuff2, 1);
	buffer_write_uint8(&netwbuff2, 6);
	buffer_write_uint8(&netwbuff2, 0);
	
	buffer_write_uint32(&netwbuff2, 0xaaaaaaaa);
	
	buffer_write_uint16(&netwbuff2, 0);
	buffer_write_uint16(&netwbuff2, 0);
	
	buffer_write_uint32(&netwbuff2, 0);
	buffer_write_uint32(&netwbuff2, 0);
	buffer_write_uint32(&netwbuff2, 0);
	buffer_write_uint32(&netwbuff2, 0);
	buffer_write(&netwbuff2, 6, &mac_addr[0]);
	buffer_zero(&netwbuff2, 2);
	buffer_zero(&netwbuff2, 8);
	buffer_zero(&netwbuff2, 192);
}

void dhcp_discover() {
//	buffer_flush(&netwbuff1);
	dhcp_bootp_init(&netwbuff2, 0x00000000);
	buffer_write_uint32(&netwbuff2, htonl(DHCP_MAGIC));

	buffer_write_uint8(&netwbuff2, DHCP_OPT_TYPE);
	buffer_write_uint8(&netwbuff2, 0x1);
	buffer_write_uint8(&netwbuff2, DHCP_DISCOVER);
	buffer_write_uint8(&netwbuff2, DHCP_OPT_END);
//	eth_write_header(&netwbuff1, mac_addr, mac_bcast, ETH_IPV4);
	ip_write_udp_message(&netwbuff1, 0x00000000, 0xFFFFFFFF, 68, 67, 
							buffer_len(&netwbuff2), netwbuff2.start);
	net_send();

}

void dhcp_request(uint32_t siaddr, uint32_t req) {
//	buffer_flush(&netwbuff1);
	dhcp_bootp_init(&netwbuff2, siaddr);
	buffer_write_uint32(&netwbuff2, htonl(DHCP_MAGIC));
	buffer_write_uint8(&netwbuff2, DHCP_OPT_TYPE);
	buffer_write_uint8(&netwbuff2, 1);
	buffer_write_uint8(&netwbuff2, DHCP_REQUEST);
	
	buffer_write_uint8(&netwbuff2, DHCP_OPT_REQUEST);
	buffer_write_uint8(&netwbuff2, 4);
	buffer_write_uint32(&netwbuff2, req);

	buffer_write_uint8(&netwbuff2, DHCP_OPT_END);
	ip_write_udp_message(&netwbuff1, 0x00000000, 0xFFFFFFFF, 68,67,
							buffer_len(&netwbuff2), netwbuff2.start);
	net_send();
}

uint32_t dhcp_get_yiaddr() {
	return htonl(*((uint32_t*)&net_buffer[0x3A]));
}

uint32_t dhcp_get_siaddr() {
	return htonl(*((uint32_t*)&net_buffer[0x3E]));
}

uint32_t dhcp_get_opt(uint8_t opt) {
	uint32_t res = 0;
	uint8_t* magic = &net_buffer[0x11A];
	for(; *magic != DHCP_OPT_END;) {
		if(*magic == opt) {
			magic++;
			uint8_t x;
			uint8_t len = *magic;
			magic++;
			for(x = 0;x < len; magic++, x++){
				((char*)&res)[x] = *magic;
			}
			return res;
		}
		else {
			magic += magic[1];
			magic++;
			magic++;
		}
	}
	return res;
}

int dhcp_is_magic() {
	return DHCP_MAGIC == (*(uint32_t*)&net_buffer[0x116]);
}

