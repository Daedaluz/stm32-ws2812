#include <stdio.h>
#include <stm32f10x.h>
#include <stm32f10x_dma.h>
#include <stm32f10x_tim.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <string.h>
#include "write.h"

static GPIO_InitTypeDef pwm_out = {
	.GPIO_Pin = GPIO_Pin_6,
	.GPIO_Speed = GPIO_Speed_50MHz,
//	.GPIO_Mode = GPIO_Mode_Out_PP
	.GPIO_Mode = GPIO_Mode_AF_PP
};

static DMA_InitTypeDef dma = {
	.DMA_PeripheralBaseAddr = (uint32_t)&TIM4->CCR1,
	.DMA_MemoryBaseAddr = (uint32_t)(&write_buffer[0]),
	.DMA_DIR = DMA_DIR_PeripheralDST,
	.DMA_BufferSize = BUFFER_SIZE,
	.DMA_PeripheralInc = DMA_PeripheralInc_Disable,
	.DMA_MemoryInc = DMA_MemoryInc_Enable,
	.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord,
	.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord,
	.DMA_Mode = DMA_Mode_Circular,
	.DMA_Priority = DMA_Priority_High,
	.DMA_M2M = DMA_M2M_Disable
};

static TIM_TimeBaseInitTypeDef tim3 = {
//	.TIM_Prescaler = 2,
	.TIM_Prescaler = 8,
//	.TIM_Period = 29,
	.TIM_Period = 9,
	.TIM_ClockDivision = TIM_CKD_DIV1,
	.TIM_CounterMode = TIM_CounterMode_Up
};
static TIM_OCInitTypeDef pwm = {
	.TIM_OCMode = TIM_OCMode_PWM1,
	.TIM_OutputState = TIM_OutputState_Enable,
	.TIM_Pulse = 0,
	.TIM_OCPolarity = TIM_OCPolarity_High
};

void ws2812_init() {

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	GPIO_Init(GPIOB, &pwm_out);
	
//	TIM_TIxExternalClockConfig(TIM4, TIM_TIxExternalCLK1Source_TI2, TIM_ICPolarity_Rising, 0x0);
	TIM_TimeBaseInit(TIM4, &tim3);

	TIM_OCStructInit(&pwm);
//	TIM_SetCounter(TIM4, 10000);
	pwm.TIM_OCMode = TIM_OCMode_PWM1;
	pwm.TIM_OutputState = TIM_OutputState_Enable;
//	pwm.TIM_Pulse = 0;
//	pwm.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC1Init(TIM4, &pwm);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	DMA_Init(DMA1_Channel1, &dma);
	TIM_DMACmd(TIM4, TIM_DMA_CC1, ENABLE);
	
	DMA_Cmd(DMA1_Channel1, ENABLE);
	TIM_Cmd(TIM4, ENABLE);
	
//	DMA_SetCurrDataCounter(DMA1_Channel1, BUFFER_SIZE);
//	DMA_Cmd(DMA1_Channel1, ENABLE);
//	TIM_Cmd(TIM4, ENABLE);
}

//#define CMPH (uint16_t)17
//#define CMPL (uint16_t)9
//void ws2812_write(uint32_t i, char* ptr) {
//	ws2812_buffer[(i*24)+0] = (ptr[0] & 0x80) ? CMPH:CMPL;
//	ws2812_buffer[(i*24)+1] = (ptr[0] & 0x40) ? CMPH:CMPL;
//	ws2812_buffer[(i*24)+2] = (ptr[0] & 0x20) ? CMPH:CMPL;
//	ws2812_buffer[(i*24)+3] = (ptr[0] & 0x10) ? CMPH:CMPL;
//	ws2812_buffer[(i*24)+4] = (ptr[0] & 0x08) ? CMPH:CMPL;
//	ws2812_buffer[(i*24)+5] = (ptr[0] & 0x04) ? CMPH:CMPL;
//	ws2812_buffer[(i*24)+6] = (ptr[0] & 0x02) ? CMPH:CMPL;
//	ws2812_buffer[(i*24)+7] = (ptr[0] & 0x01) ? CMPH:CMPL;
//
//	ws2812_buffer[(i*24)+8] = (ptr[1] & 0x80) ? CMPH:CMPL;
//	ws2812_buffer[(i*24)+9] = (ptr[1] & 0x40) ? CMPH:CMPL;
//	ws2812_buffer[(i*24)+10] = (ptr[1] & 0x20) ? CMPH:CMPL;
//	ws2812_buffer[(i*24)+11] = (ptr[1] & 0x10) ? CMPH:CMPL;
//	ws2812_buffer[(i*24)+12] = (ptr[1] & 0x08) ? CMPH:CMPL;
//	ws2812_buffer[(i*24)+13] = (ptr[1] & 0x04) ? CMPH:CMPL;
//	ws2812_buffer[(i*24)+14] = (ptr[1] & 0x02) ? CMPH:CMPL;
//	ws2812_buffer[(i*24)+15] = (ptr[1] & 0x01) ? CMPH:CMPL;
//	
//	ws2812_buffer[(i*24)+16] = (ptr[2] & 0x80) ? CMPH:CMPL;
//	ws2812_buffer[(i*24)+17] = (ptr[2] & 0x40) ? CMPH:CMPL;
//	ws2812_buffer[(i*24)+18] = (ptr[2] & 0x20) ? CMPH:CMPL;
//	ws2812_buffer[(i*24)+19] = (ptr[2] & 0x10) ? CMPH:CMPL;
//	ws2812_buffer[(i*24)+20] = (ptr[2] & 0x08) ? CMPH:CMPL;
//	ws2812_buffer[(i*24)+21] = (ptr[2] & 0x04) ? CMPH:CMPL;
//	ws2812_buffer[(i*24)+22] = (ptr[2] & 0x02) ? CMPH:CMPL;
//	ws2812_buffer[(i*24)+23] = (ptr[2] & 0x01) ? CMPH:CMPL;
//}

