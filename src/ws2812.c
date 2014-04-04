#include <stdio.h>
#include <stm32f10x.h>
#include <stm32f10x_dma.h>
#include <stm32f10x_tim.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <string.h>

#define LEDS 1

static uint8_t ws2812_buffer[LEDS*24+42];
//static uint16_t ws2812_buffer[LEDS+2][24];

static GPIO_InitTypeDef pwm_out = {
	.GPIO_Pin = GPIO_Pin_6,
	.GPIO_Speed = GPIO_Speed_50MHz,
	.GPIO_Mode = GPIO_Mode_AF_PP
};

static DMA_InitTypeDef dma = {
	.DMA_PeripheralBaseAddr = (uint32_t)&TIM4->CCR1,// TIM3_CCR1_Address, //TIM3_BASE,
	.DMA_MemoryBaseAddr = (uint32_t)(&ws2812_buffer[0]),
	.DMA_DIR = DMA_DIR_PeripheralDST,
	.DMA_BufferSize = LEDS*24+42, //LEDS*24*sizeof(uint16_t)+20,//42*2,
	.DMA_PeripheralInc = DMA_PeripheralInc_Disable,
	.DMA_MemoryInc = DMA_MemoryInc_Enable,
	.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord,
	.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte,
	.DMA_Mode = DMA_Mode_Circular,
//	.DMA_Mode = DMA_Mode_Normal,
	.DMA_Priority = DMA_Priority_VeryHigh,
	.DMA_M2M = DMA_M2M_Disable
};

static TIM_TimeBaseInitTypeDef tim3 = {
	.TIM_Prescaler = 2,
	.TIM_Period = 29,
	.TIM_ClockDivision = 0,
	.TIM_CounterMode = TIM_CounterMode_Up
};
static TIM_OCInitTypeDef pwm = {
	.TIM_OCMode = TIM_OCMode_PWM1,
	.TIM_OutputNState = TIM_OutputNState_Disable,
	.TIM_OutputState = TIM_OutputState_Enable,
	.TIM_Pulse = 0,
	.TIM_OCNIdleState = TIM_OCNIdleState_Reset,
	.TIM_OCIdleState = TIM_OCIdleState_Reset,
	.TIM_OCNPolarity = TIM_OCPolarity_High,
	.TIM_OCPolarity = TIM_OCPolarity_High
};


//void ws2812_write(uint32_t i, char r, char g, char b);
void ws2812_write(uint32_t i, char*);


static int x = 0;
static char omg[3] = {0x0f, 0xff, 0x00};
void ws2812_init() {
//	tim3.TIM_Prescaler = (uint16_t)(SystemCoreClock / 24000000) - 1;

	for(x=0; x < 24; x++)
		ws2812_buffer[x] = 1;
	int size = ((LEDS)*24)*sizeof(uint16_t);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	GPIO_Init(GPIOB, &pwm_out);
	
	TIM_TimeBaseInit(TIM4, &tim3);
	TIM_OC1Init(TIM4, &pwm);
//	TIM_ITConfig(TIM4, TIM_IT_CC1, DISABLE);
//	TIM_DMAConfig(TIM4, TIM_DMABase_CCR1, TIM_DMABurstLength_18Transfers);
//	TIM_InternalClockConfig(TIM4);
//	TIM_OC1FastConfig(TIM3, TIM_OCFast_Enable);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	DMA_Init(DMA1_Channel1, &dma);
	TIM_DMACmd(TIM4, TIM_DMA_CC1, ENABLE);
	
	DMA_Cmd(DMA1_Channel1, ENABLE);
	TIM_Cmd(TIM4, ENABLE);
	char buff[500];
	for(x = 0; x < LEDS; x++) {
		ws2812_write(x, omg);
		cputc('.');
	}
	cputc('\n');
//	ws2812_write(0, 0x10, 0, 0);
//	sprintf(buff, "size: %p %p - %d %p %d\n", &ws2812_buffer[0][0], &ws2812_buffer[0][1], sizeof(ws2812_buffer, &x, x));
//	cputs(buff);
}

#define CMPH (uint16_t)17
#define CMPL (uint16_t)9
//#define CMPH (uint16_t)(22+5)
//#define CMPL (uint16_t)(22-5)
/*
static void printled(int i) {
	char buff[500];
	sprintf(buff, "%d %p: %d %d %d %d %d %d %d %d - %d %d %d %d %d %d %d %d - %d %d %d %d %d %d %d %d\n",
			i, &ws2812_buffer[i][0],
			ws2812_buffer[i][0],
			ws2812_buffer[i][1],
			ws2812_buffer[i][2],
			ws2812_buffer[i][3],
			ws2812_buffer[i][4],
			ws2812_buffer[i][5],
			ws2812_buffer[i][6],
			ws2812_buffer[i][7],
			ws2812_buffer[i][8],
			ws2812_buffer[i][9],
			ws2812_buffer[i][10],
			ws2812_buffer[i][11],
			ws2812_buffer[i][12],
			ws2812_buffer[i][13],
			ws2812_buffer[i][14],
			ws2812_buffer[i][15],
			ws2812_buffer[i][16],
			ws2812_buffer[i][17],
			ws2812_buffer[i][18],
			ws2812_buffer[i][19],
			ws2812_buffer[i][20],
			ws2812_buffer[i][21],
			ws2812_buffer[i][22],
			ws2812_buffer[i][23],
			ws2812_buffer[i][24]);
	cputs(buff);
}
*/
void ws2812_write(uint32_t i, char* ptr) {
	ws2812_buffer[(i*24)+0] = (ptr[0] & 0x80) ? CMPH:CMPL;
	ws2812_buffer[(i*24)+1] = (ptr[0] & 0x40) ? CMPH:CMPL;
	ws2812_buffer[(i*24)+2] = (ptr[0] & 0x20) ? CMPH:CMPL;
	ws2812_buffer[(i*24)+3] = (ptr[0] & 0x10) ? CMPH:CMPL;
	ws2812_buffer[(i*24)+4] = (ptr[0] & 0x08) ? CMPH:CMPL;
	ws2812_buffer[(i*24)+5] = (ptr[0] & 0x04) ? CMPH:CMPL;
	ws2812_buffer[(i*24)+6] = (ptr[0] & 0x02) ? CMPH:CMPL;
	ws2812_buffer[(i*24)+7] = (ptr[0] & 0x01) ? CMPH:CMPL;

	ws2812_buffer[(i*24)+8] = (ptr[1] & 0x80) ? CMPH:CMPL;
	ws2812_buffer[(i*24)+9] = (ptr[1] & 0x40) ? CMPH:CMPL;
	ws2812_buffer[(i*24)+10] = (ptr[1] & 0x20) ? CMPH:CMPL;
	ws2812_buffer[(i*24)+11] = (ptr[1] & 0x10) ? CMPH:CMPL;
	ws2812_buffer[(i*24)+12] = (ptr[1] & 0x08) ? CMPH:CMPL;
	ws2812_buffer[(i*24)+13] = (ptr[1] & 0x04) ? CMPH:CMPL;
	ws2812_buffer[(i*24)+14] = (ptr[1] & 0x02) ? CMPH:CMPL;
	ws2812_buffer[(i*24)+15] = (ptr[1] & 0x01) ? CMPH:CMPL;
	
	ws2812_buffer[(i*24)+16] = (ptr[2] & 0x80) ? CMPH:CMPL;
	ws2812_buffer[(i*24)+17] = (ptr[2] & 0x40) ? CMPH:CMPL;
	ws2812_buffer[(i*24)+18] = (ptr[2] & 0x20) ? CMPH:CMPL;
	ws2812_buffer[(i*24)+19] = (ptr[2] & 0x10) ? CMPH:CMPL;
	ws2812_buffer[(i*24)+20] = (ptr[2] & 0x08) ? CMPH:CMPL;
	ws2812_buffer[(i*24)+21] = (ptr[2] & 0x04) ? CMPH:CMPL;
	ws2812_buffer[(i*24)+22] = (ptr[2] & 0x02) ? CMPH:CMPL;
	ws2812_buffer[(i*24)+23] = (ptr[2] & 0x01) ? CMPH:CMPL;
//void ws2812_write(uint32_t i, char r, char g, char b) {
//	cputc(' ');
//	cputc(' ');
//	cputc(' ');
//	cputc(' ');
//	cputc(' ');
//	cputc(' ');
//	cputc(' ');
//	cputc(' ');
//	cputc(' ');
//	cputc(' ');
//	cputc(' ');
//	cputc(' ');
//	cputc(' ');
//	cputc(' ');
//	for (x = 0; x < 8; x++) {
		//if((r << x) & 0x80){
//		if((ptr[0] << x) & 0x80){
//			ws2812_buffer[i][x] = CMPH;
//			ws2812_buffer[(i*24)+x] = CMPH;
//			cputc('H');
//			cputc(' ');
//		}
//		else{
//			ws2812_buffer[i][x] = CMPL;
//			ws2812_buffer[(i*24)+x] = CMPL;
//			cputc('L');
//			cputc(' ');
//		}
//	}
//	cputc('-');
//	cputc(' ');
//	for (x = 0; x < 8; x++) {
		//if((g << x) & 0x80){
//		if((ptr[1] << x) & 0x80){
//			ws2812_buffer[i][x + 8] = CMPH;
//			ws2812_buffer[(i*24)+x+8] = CMPH;
//			cputc('H');
//			cputc(' ');
//		}
//		else {
//			ws2812_buffer[i][x + 8] = CMPL;
//			ws2812_buffer[(i*24)+x+8] = CMPL;
//			cputc('L');
//			cputc(' ');
//		}
//	}
//	cputc('-');
//	cputc(' ');

//	for (x = 0; x < 8; x++) {
//		//if((b << x) & 0x80){
//		if((ptr[2] << x) & 0x80){
//			ws2812_buffer[i][x + 16] = CMPH;
//			ws2812_buffer[(i*24)+x+16] = CMPH;
//			cputc('H');
//			cputc(' ');
//		}
//		else{
//			ws2812_buffer[i][x + 16] = CMPL;
//			ws2812_buffer[(i*24)+x+16] = CMPL;
//			cputc('L');
//			cputc(' ');
//		}
//	}
//	cputc('\n');
//	printled(i);
}

