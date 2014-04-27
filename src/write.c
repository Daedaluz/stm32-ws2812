#include <stm32f10x_dma.h>
#include <stm32f10x_tim.h>
#include "write.h"
#include <stm32f10x_gpio.h>

// 10 Meters of ws2812
// also fits dmx (of 513)
// and ws2801 ;)

uint16_t write_buffer[BUFFER_SIZE];

void request_write_dmx(void) {
	// allways write 512 bytes
}
#define PERIOD 3
#define HIGH 2
#define LOW 1
#define FAC 1
void request_write_ws2812(void) {
//	while(DMA_GetFlagStatus(DMA1_FLAG_TC1));
//	DMA_SetCurrDataCounter(DMA1_Channel1, BUFFER_SIZE);
//	DMA_Cmd(DMA1_Channel1, ENABLE);
//	TIM_Cmd(TIM4, ENABLE);
//	while(!DMA_GetFlagStatus(DMA1_FLAG_TC1));
//	TIM_Cmd(TIM4, DISABLE);
//	DMA_Cmd(DMA1_Channel1, DISABLE);
//	DMA_ClearFlag(DMA1_FLAG_TC1);
	
//	int x = 0;
//	int y = 0;
//	for(x = 0; x < ((17*24)+42); x++) {
//		switch(write_buffer[x]) {
//			case 17:
//				GPIO_WriteBit(GPIOB,GPIO_Pin_6,1);
//				for(y = 0; y <= (HIGH*FAC); y++){}
//				GPIO_WriteBit(GPIOB,GPIO_Pin_6,0);
//				for(y = 0; y <= ((PERIOD-HIGH)*FAC); y++){}
//				break;
//			case 9:
//				GPIO_WriteBit(GPIOB,GPIO_Pin_6,1);
//				for(y = 0; y <= (LOW*FAC); y++){}
//				GPIO_WriteBit(GPIOB,GPIO_Pin_6,0);
//				for(y = 0; y <= ((PERIOD-LOW)*FAC); y++){}
//				break;
//			case 0:
//				GPIO_WriteBit(GPIOB, GPIO_Pin_6, 0);
//				for(y = 0; y <= (PERIOD*FAC); y++){}
//				break;
//		}
//	}
}

void request_write_ws2801(void) {

}

