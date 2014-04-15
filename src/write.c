#include <stm32f10x_dma.h>
#include <stm32f10x_tim.h>
#include "write.h"

// 10 Meters of ws2812
// also fits dmx (of 513)
// and ws2801 ;)

uint8_t write_buffer[BUFFER_SIZE];

void request_write_dmx(void) {
	// allways write 512 bytes
}

void request_write_ws2812(void) {
	while(!DMA_GetFlagStatus(DMA1_FLAG_TC1));
	TIM_Cmd(TIM4, DISABLE);
	DMA_Cmd(DMA1_Channel1, DISABLE);
	DMA_ClearFlag(DMA1_FLAG_TC1);

	DMA_SetCurrDataCounter(DMA1_Channel1, BUFFER_SIZE);
	DMA_Cmd(DMA1_Channel1, ENABLE);
	TIM_Cmd(TIM4, ENABLE);
}

void request_write_ws2801(void) {

}

