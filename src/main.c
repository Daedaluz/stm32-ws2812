#include <stdio.h>
#include <string.h>
#include <stm32f10x.h>
#include <stm32f10x_dma.h>
#include <stm32f10x_tim.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>
#include <enc28j60_driver.h>

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

static int i = 0;
static int x = 0;

static uint32_t mode = 0;

int main() {
	uart1_init();
	ws2812_init();
	e28j60_init();
	GPIO_Init(GPIOB, &gpiocfg);
	GPIO_WriteBit(GPIOB, GPIO_Pin_12, 1);
	char buffer[] = {0xaa, 0x90, 0xff, 0x00, 0x10};
	while(1)
		e28j60_wbm_cont(buffer, 5);
//		e28j60_spi_rw(0x01);
	cputc('x');
//	sprintf(lol, "%d\n", SystemCoreClock);
//	cputs(lol);
	int i;
	while(1){
		for(i = 0; i < 7200000; i++);
		request_write_ws2812();
	}
}


