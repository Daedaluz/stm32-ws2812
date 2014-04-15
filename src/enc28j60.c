#include <stm32f10x.h>
#include <stm32f10x_spi.h>
#include <stm32f10x_gpio.h>
#include <misc.h>
#include <stdint.h>

static GPIO_InitTypeDef spi_gpio_out = {
	.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7,
	.GPIO_Speed = GPIO_Speed_50MHz,
	.GPIO_Mode = GPIO_Mode_AF_PP
};
static GPIO_InitTypeDef spi_gpio_in = {
	.GPIO_Pin = GPIO_Pin_6,
	.GPIO_Speed = GPIO_Speed_50MHz,
	.GPIO_Mode = GPIO_Mode_IN_FLOATING
};

static GPIO_InitTypeDef spi_cs = {
	.GPIO_Pin = GPIO_Pin_1,
	.GPIO_Speed = GPIO_Speed_50MHz,
	.GPIO_Mode = GPIO_Mode_Out_PP
};

static GPIO_InitTypeDef led4 = {
	.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14,
	.GPIO_Speed = GPIO_Speed_50MHz,
	.GPIO_Mode = GPIO_Mode_Out_PP
};

static SPI_InitTypeDef spi_init = {
	.SPI_Direction = SPI_Direction_2Lines_FullDuplex,
	.SPI_Mode = SPI_Mode_Master,
	.SPI_DataSize = SPI_DataSize_8b,
	.SPI_CPOL = SPI_CPOL_Low,
	.SPI_CPHA = SPI_CPHA_1Edge,
	.SPI_NSS = SPI_NSS_Soft,
	.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4,
	.SPI_FirstBit = SPI_FirstBit_MSB,
};

static EXTI_InitTypeDef exit_int = {
	.EXTI_Line = EXTI_Line6 | EXTI_Line7 | EXTI_Line8,
	.EXTI_Mode = EXTI_Mode_Interrupt,
	.EXTI_Trigger = EXTI_Trigger_Rising_Falling,
	.EXTI_LineCmd = ENABLE
};

static NVIC_InitTypeDef nvic = {
	.NVIC_IRQChannel = EXTI9_5_IRQn,
	.NVIC_IRQChannelPreemptionPriority = 10,
	.NVIC_IRQChannelSubPriority = 10,
	.NVIC_IRQChannelCmd = ENABLE
};

static GPIO_InitTypeDef btn = {
	.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8,
	.GPIO_Speed = GPIO_Speed_50MHz,
	.GPIO_Mode = GPIO_Mode_IN_FLOATING
};

/*
void EXTI9_5_IRQHandler(void) {
	led_status = !led_status;
	GPIO_WriteBit(GPIOB, GPIO_Pin_15, led_status);
	EXTI_ClearITPendingBit(EXTI_Line6);
	EXTI_ClearITPendingBit(EXTI_Line7);
	EXTI_ClearITPendingBit(EXTI_Line8);
}*/

//void SysTick_Handler(void) {
//	led_status = !led_status;
//	GPIO_WriteBit(GPIOB, GPIO_Pin_15, led_status);
//}


uint8_t e28j60_spi_rw(const uint8_t c) {
//	SPI_I2S_SendData(SPI1, c);
	SPI1->DR = c;
	while( !(SPI1->SR & SPI_I2S_FLAG_TXE) );
	while( !(SPI1->SR & SPI_I2S_FLAG_RXNE) );
	while( SPI1->SR & SPI_I2S_FLAG_BSY );
	return SPI1->DR;
//	return (uint8_t)SPI_I2S_ReceiveData(SPI1);
}

void e28j60_spi_cs_on(void) {
	GPIO_WriteBit(GPIOB, GPIO_Pin_1, 0);
}

void e28j60_spi_cs_off(void) {
	GPIO_WriteBit(GPIOB, GPIO_Pin_1, 1);
}

void e28j60_init(void) {
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | 
//			RCC_APB2Periph_SPI1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOC | 
			RCC_APB2Periph_SPI1, ENABLE);
	GPIO_Init(GPIOB, &spi_cs);
	GPIO_Init(GPIOB, &led4);
	GPIO_Init(GPIOA, &spi_gpio_out);
	GPIO_Init(GPIOA, &spi_gpio_in);
//	GPIO_Init(GPIOC, &btn);
	SPI_Init(SPI1, &spi_init);
	SPI_Cmd(SPI1, ENABLE);

	GPIO_WriteBit(GPIOB, GPIO_Pin_15, 1);

//	SysTick_Config(16000000);
	SysTick_Config(0xefffff);

//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource10);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource6);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource7);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource8);
	NVIC_Init(&nvic);
}

