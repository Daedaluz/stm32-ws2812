STDLIB=lib/src/core_cm3.o lib/src/misc.o lib/src/stm32f10x_adc.o lib/src/stm32f10x_bkp.o lib/src/stm32f10x_can.o lib/src/stm32f10x_cec.o lib/src/stm32f10x_crc.o lib/src/stm32f10x_dac.o lib/src/stm32f10x_dma.o lib/src/stm32f10x_exti.o lib/src/stm32f10x_flash.o lib/src/stm32f10x_fsmc.o lib/src/stm32f10x_gpio.o lib/src/stm32f10x_i2c.o lib/src/stm32f10x_ip_dbg.o lib/src/stm32f10x_it.o lib/src/stm32f10x_iwdg.o lib/src/stm32f10x_pwr.o lib/src/stm32f10x_rcc.o lib/src/stm32f10x_rtc.o lib/src/stm32f10x_sdio.o lib/src/stm32f10x_spi.o lib/src/stm32f10x_tim.o lib/src/stm32f10x_usart.o lib/src/stm32f10x_wwdg.o lib/src/system_stm32f10x.o

SRC=src/main.o src/uart.o src/ws2812.o

CC=arm-none-eabi-gcc
CXX=arm-none-eabi-gcc
AS=arm-none-eabi-as
LD=arm-none-eabu-ld


SYS=MD
HSE=8000000

STARTUP=lib/startup/startup_stm32f10x_$(shell echo $(SYS) | tr A-Z a-z).o

CFLAGS=-mcpu=cortex-m3 -mthumb -mlittle-endian -mthumb-interwork -T lib/ld/stm32_flash.ld -DUSE_STDPERIPH_DRIVER -DSTM32F10X_$(SYS) -DHSE_VALUE=$(HSE) -Ilib/inc  -lnosys -nostdlib -Os
LDFLAGS=-lnosys -nostdlib -lc -lm

main.elf: $(STDLIB) $(SRC) $(STARTUP)
	$(CC) $(SRC) $(STARTUP) $(STDLIB) $(CFLAGS) -o main.elf

.PHONY: clean

clean:
	find . -name "*.o" | xargs rm

