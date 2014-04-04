#include <stdio.h>
#include <string.h>
#include <stm32f10x.h>
#include <stm32f10x_dma.h>
#include <stm32f10x_tim.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>

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

static char buff[3] = {0x0f,0xff,0};
static char lol[200];
static int i = 0;
static int x = 0;

int main() {
	uart1_init();
	ws2812_init();

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_Init(GPIOB, &gpiocfg);
	GPIO_WriteBit(GPIOB, GPIO_Pin_12, 1);

	cputc('x');
	sprintf(lol, "%d\n", SystemCoreClock);
	cputs(lol);
	int i;
	ws2812_write(0, buff);
	while(1){
//		ws2812_write(0, buff);
//		buff[1]++;
		for(i = 0; i < 7200000; i++);
//		buff[0]++;
//		for(i = 0; i < 1; i++) {
//			for(x = 0; x < 3; x++) {
//				buff[x] = cgetc();
//				cputc(buff[x]);
//			}
//		ws2812_write(0, buff[1], buff[0], buff[2]);
//			cputc('p');
//		}
	}
}


