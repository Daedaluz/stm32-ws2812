#include <stm32f10x_usart.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
static GPIO_InitTypeDef usart1_rx = {
	.GPIO_Pin = GPIO_Pin_10,
//	.GPIO_Speed = GPIO_Speed_50MHz,
	.GPIO_Mode = GPIO_Mode_IN_FLOATING
};
static GPIO_InitTypeDef usart1_tx = {
	.GPIO_Pin = GPIO_Pin_9,
	.GPIO_Speed = GPIO_Speed_50MHz,
	.GPIO_Mode = GPIO_Mode_AF_PP
};

static USART_InitTypeDef usart = {
//	.USART_BaudRate = 235000,
	.USART_BaudRate = 115200,
	.USART_WordLength = USART_WordLength_8b,
	.USART_StopBits = USART_StopBits_1,
	.USART_Parity = USART_Parity_No,
	.USART_Mode = USART_Mode_Tx | USART_Mode_Rx,
	.USART_HardwareFlowControl = USART_HardwareFlowControl_None
};

void uart1_init() {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, 
			ENABLE);
	GPIO_Init(GPIOA, &usart1_rx);
	GPIO_Init(GPIOA, &usart1_tx);
	
	USART_Init(USART1, &usart);
	USART_Cmd(USART1, ENABLE);
	return;
}

void cputc(uint8_t c)
{
	USART_SendData(USART1, c);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

uint16_t cgetc() {
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
	uint16_t x = USART_ReceiveData(USART1);
	return x;
}
void cputs(const char* s) {
	while(*s) 
		cputc(*s++);
}

