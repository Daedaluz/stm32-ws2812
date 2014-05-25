#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stm32f10x.h>
#include <stm32f10x_dma.h>
#include <stm32f10x_tim.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>
#include <enc28j60.h>
#include "write.h"
#include "buffer.h"
#include "eth.h"
#include "ip.h"
#include "dhcp.h"
#include "net.h"
#include "arp.h"
#include "sleep.h"

#include "uart.h"
#include "ws2812.h"


static GPIO_InitTypeDef gpiocfg = {
	.GPIO_Pin = GPIO_Pin_12,
	.GPIO_Speed = GPIO_Speed_50MHz,
	.GPIO_Mode = GPIO_Mode_Out_PP
};

int _sbrk(int a) {
	return a;
}

static uint8_t mac[6] = {0x54, 0x55, 0x58, 0x10, 0x00, 0x01};

int main() {

	uart1_init();
	ws2812_init();
	GPIO_Init(GPIOB, &gpiocfg);
	char buffer[] = {0xaa, 0x90, 0xff, 0x00, 0x10};
	char buff[50];
	
	SPI1_Init();
	enc28j60Init(mac_addr);
	enc28j60PhyWrite(PHLCON, 0x7a4);
	enc28j60clkout(2);
	net_init(mac);
	msleep(20);
	
	write_ws2812(0,3, "\x00\x0f\x00");


	msleep(50);
	buffer_flush(&netwbuff1);
	eth_write_header(&netwbuff1, mac_addr, mac_bcast, ETH_IPV4);
	dhcp_discover();
	while(1) {
		net_recv();
		if(eth_get_type() == ETH_IPV4 && ip_get_protocol() == IP_UDP && ip_udp_get_dst() == 68 && dhcp_is_magic())
			break;
	}
	{
		uint32_t opt = dhcp_get_opt(DHCP_OPT_TYPE);
		char tmpmac[6];
		uint32_t tmpyip = dhcp_get_yiaddr();
		uint32_t tmpsip = dhcp_get_siaddr();
		memcpy(tmpmac, eth_get_src(), 6);
		buffer_flush(&netwbuff1);
		eth_write_header(&netwbuff1, &mac_addr[0], tmpmac, ETH_IPV4);
		dhcp_request(tmpsip, tmpyip);
//		cputs(buff);
	}
	while(1){
		net_recv();
		{
			if(eth_get_type() == ETH_IPV4 && ip_get_protocol() == IP_UDP && ip_udp_get_dst() == 68 && 
				dhcp_is_magic() && dhcp_get_opt(DHCP_OPT_TYPE) == DHCP_ACK) {
				uint32_t my_ip = dhcp_get_yiaddr();
				net_set_ip(htonl(my_ip));
				break;
			}
		}
	}
	GPIO_WriteBit(GPIOB, GPIO_Pin_12, 1);
	msleep(1);

	arp_gratuitous(&netwbuff1);
	net_send();
	buffer_flush(&netwbuff1);
	while(1){
		net_recv();
		switch(eth_get_type()) {
			case ETH_IPV4:
				// suck all the led-data!!!
				if(ip_get_dst_addr() == htonl(*(uint32_t*)ip_addr) && ip_udp_get_dst() == 1200){
					write_ws2812(0, (uint32_t)ip_udp_get_len(), ip_udp_get_datap());
					
				}
			break;
			case ETH_ARP:{
				//answer the damn arp request
				if(arp_get_target_ip() == htonl(*(uint32_t*)ip_addr)) {
					memcpy(&mac[0], eth_get_src(), 6);
					uint32_t tmpip = arp_get_sender_ip();
					arp_reply(&netwbuff1, mac, tmpip);
					net_send();
				}
			}
			break;
			default:
				cputs("EH?!\n");
			break;
		}
	}
}


