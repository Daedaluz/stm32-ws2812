#include <stm32f10x_spi.h>
#include <stm32f10x_gpio.h>

static GPIO_InitTypeDef spi_cs = {
	.GPIO_Pin = GPIO_Pin_4,
	.GPIO_Speed = GPIO_Speed_50MHz,
	.GPIO_Mode = GPIO_Mode_Out_PP
};

void ENC28J60_CSL(void) {
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, 0);
}

void ENC28J60_CSH(void) {
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, 1);
}

void SPI1_Init(void) 
{ 
	SPI_InitTypeDef  SPI_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure; 

	/* Enable SPI1 and GPIOA clocks */ 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE); 

	/* Configure SPI1 pins: NSS, SCK, MISO and MOSI */ 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	GPIO_Init(GPIOA, &spi_cs);

	/* SPI1 configuration */  
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; 
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; 
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; 
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; 
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; 
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; 
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; 
	SPI_InitStructure.SPI_CRCPolynomial = 7; 
	SPI_Init(SPI1, &SPI_InitStructure); 

	/* Enable SPI1  */ 
	SPI_Cmd(SPI1, ENABLE);  
} 

//SPI1读写一字节数据 
uint8_t	SPI1_ReadWrite(uint8_t writedat) 
{ 
	/* Loop while DR register in not emplty */ 
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); 

	/* Send byte through the SPI1 peripheral */ 
	SPI_I2S_SendData(SPI1, writedat); 

	/* Wait to receive a byte */ 
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET); 

	/* Return the byte read from the SPI bus */ 
	return SPI_I2S_ReceiveData(SPI1); 
} 

