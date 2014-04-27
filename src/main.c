#include <stdio.h>
#include <string.h>
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

static int i = 0;
static int x = 0;

uint8_t mac[] = {0x54,0x55,0x58,0x10,0x00,0x01};
uint8_t ip[] = {192,168,0,4};
uint8_t recv_buff[1024];

//#define CMPH 17
//#define CMPL 9

#define CMPH 8
#define CMPL 2

void write_lamp(int l, uint8_t g, uint8_t r, uint8_t b) {
	write_buffer[(x*24)+0] = (g & 0x80) ? CMPH:CMPL;
	write_buffer[(x*24)+1] = (g & 0x40) ? CMPH:CMPL;
	write_buffer[(x*24)+2] = (g & 0x20) ? CMPH:CMPL;
	write_buffer[(x*24)+3] = (g & 0x10) ? CMPH:CMPL;
	write_buffer[(x*24)+4] = (g & 0x08) ? CMPH:CMPL;
	write_buffer[(x*24)+5] = (g & 0x04) ? CMPH:CMPL;
	write_buffer[(x*24)+6] = (g & 0x02) ? CMPH:CMPL;
	write_buffer[(x*24)+7] = (g & 0x01) ? CMPH:CMPL;

	write_buffer[(x*24)+8] = (r & 0x80) ? CMPH:CMPL;
	write_buffer[(x*24)+9] = (r & 0x40) ? CMPH:CMPL;
	write_buffer[(x*24)+10] = (r & 0x20) ? CMPH:CMPL;
	write_buffer[(x*24)+11] = (r & 0x10) ? CMPH:CMPL;
	write_buffer[(x*24)+12] = (r & 0x08) ? CMPH:CMPL;
	write_buffer[(x*24)+13] = (r & 0x04) ? CMPH:CMPL;
	write_buffer[(x*24)+14] = (r & 0x02) ? CMPH:CMPL;
	write_buffer[(x*24)+15] = (r & 0x01) ? CMPH:CMPL;
	
	write_buffer[(x*24)+16] = (b & 0x80) ? CMPH:CMPL;
	write_buffer[(x*24)+17] = (b & 0x40) ? CMPH:CMPL;
	write_buffer[(x*24)+18] = (b & 0x20) ? CMPH:CMPL;
	write_buffer[(x*24)+19] = (b & 0x10) ? CMPH:CMPL;
	write_buffer[(x*24)+20] = (b & 0x08) ? CMPH:CMPL;
	write_buffer[(x*24)+21] = (b & 0x04) ? CMPH:CMPL;
	write_buffer[(x*24)+22] = (b & 0x02) ? CMPH:CMPL;
	write_buffer[(x*24)+23] = (b & 0x01) ? CMPH:CMPL;
}

int main() {
	uart1_init();
	ws2812_init();
//	e28j60_init();
	GPIO_Init(GPIOB, &gpiocfg);
	GPIO_WriteBit(GPIOB, GPIO_Pin_12, 1);
	char buffer[] = {0xaa, 0x90, 0xff, 0x00, 0x10};
	
	SPI1_Init();
	init_ip_arp_udp_tcp(mac, ip, 80);
	enc28j60Init(mac);
	enc28j60PhyWrite(PHLCON, 0x7a4);
	enc28j60clkout(2);
	Del_1ms(20);
//	simple_server();
	cputs("ws2812 setup...\nWriting initial red color to all lamps\n");

	for(x=0; x < 3; x++) {
		write_lamp(x, 0,10,0);
	}

	request_write_ws2812();
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
			cputs("writing leds...\n");
			int x;
			for(x=0; x < plen; x++) {
				write_buffer[(x*8)+0] = (recv_buff[UDP_DATA_P+x] & 0x80) ? CMPH:CMPL;
				write_buffer[(x*8)+1] = (recv_buff[UDP_DATA_P+x] & 0x40) ? CMPH:CMPL;
				write_buffer[(x*8)+2] = (recv_buff[UDP_DATA_P+x] & 0x20) ? CMPH:CMPL;
				write_buffer[(x*8)+3] = (recv_buff[UDP_DATA_P+x] & 0x10) ? CMPH:CMPL;
				write_buffer[(x*8)+4] = (recv_buff[UDP_DATA_P+x] & 0x08) ? CMPH:CMPL;
				write_buffer[(x*8)+5] = (recv_buff[UDP_DATA_P+x] & 0x04) ? CMPH:CMPL;
				write_buffer[(x*8)+6] = (recv_buff[UDP_DATA_P+x] & 0x02) ? CMPH:CMPL;
				write_buffer[(x*8)+7] = (recv_buff[UDP_DATA_P+x] & 0x01) ? CMPH:CMPL;
				char buff[100];
				sprintf(buff, "%d %d %d %d %d %d %d %d\n", write_buffer[(x*8)+0], write_buffer[(x*8)+1], write_buffer[(x*8)+2],
						write_buffer[(x*8)+3], write_buffer[(x*8)+4], write_buffer[(x*8)+5], write_buffer[(x*8)+6], write_buffer[(x*8)+7]);
				cputs(buff);
			}
			request_write_ws2812();
			cputs("done...\n");
		}

	}
}


