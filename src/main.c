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
#include <net.h>
#include "write.h"

//#include <enc28j60_driver.h>
//#include <enc28j60defs.h>

#include "uart.h"
#include "ws2812.h"

void Del_10us(){
	int l;
	for(l=0; l < 8000000; l++);
}

void Del_1ms(){
	int l;
	for(l = 0; l < 100; l++) Del_10us;
}

static GPIO_InitTypeDef gpiocfg = {
	.GPIO_Pin = GPIO_Pin_12,
	.GPIO_Speed = GPIO_Speed_50MHz,
	.GPIO_Mode = GPIO_Mode_Out_PP
};

int _sbrk(int a) {
	return a;
}


uint8_t mac[] = {0x54,0x55,0x58,0x10,0x00,0x01};
uint8_t recv_buff[1524];


int main() {
	uart1_init();
	ws2812_init();
	GPIO_Init(GPIOB, &gpiocfg);
	GPIO_WriteBit(GPIOB, GPIO_Pin_12, 1);
	char buffer[] = {0xaa, 0x90, 0xff, 0x00, 0x10};
	
	SPI1_Init();
	init_ip_arp_udp(mac);
	enc28j60Init(mac);
	enc28j60PhyWrite(PHLCON, 0x7a4);
	enc28j60clkout(2);
	Del_1ms(20);
	write_ws2812(0,3, "\x00\x0f\x00");
	cputs("ws2812 setup...\nWriting initial red color to all lamps\n");

	cputs("done.. now accepting net packets\n");
	while(1){
		Del_1ms(50);
		uint16_t len = enc28j60PacketReceive(1024, recv_buff);
		if (len == 0){
			cputs("error\n");
			continue;
		}
		
		if(eth_type_is_arp_and_my_ip(recv_buff,len)) {
			cputs("arp\n");
			make_arp_answer_from_request(recv_buff);
			continue;
		}
		
		if(eth_type_is_ip_and_my_ip(recv_buff, len) == 0) {
			cputs("Not for us..\n");
			continue;
		}

		//PING!!
		if(recv_buff[IP_PROTO_P]==IP_PROTO_ICMP_V && recv_buff[ICMP_TYPE_P] == ICMP_TYPE_ECHOREQUEST_V){
			make_echo_reply_from_request(recv_buff, len);
			cputs("ping\n");
			continue;
		}
		
		//UDP PORT 1200
		if(recv_buff[IP_PROTO_P]==IP_PROTO_UDP_V && recv_buff[UDP_DST_PORT_H_P] == 4 && recv_buff[UDP_DST_PORT_L_P] == 0xb0) {
			uint16_t plen = recv_buff[UDP_LEN_L_P] - UDP_HEADER_LEN;
			write_ws2812(0, plen, &recv_buff[UDP_DATA_P]);
		}
	}
}


