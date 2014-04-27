STDLIB=lib/src/core_cm3.o lib/src/misc.o lib/src/stm32f10x_adc.o lib/src/stm32f10x_bkp.o lib/src/stm32f10x_can.o lib/src/stm32f10x_cec.o lib/src/stm32f10x_crc.o lib/src/stm32f10x_dac.o lib/src/stm32f10x_dma.o lib/src/stm32f10x_exti.o lib/src/stm32f10x_flash.o lib/src/stm32f10x_fsmc.o lib/src/stm32f10x_gpio.o lib/src/stm32f10x_i2c.o lib/src/stm32f10x_ip_dbg.o lib/src/stm32f10x_it.o lib/src/stm32f10x_iwdg.o lib/src/stm32f10x_pwr.o lib/src/stm32f10x_rcc.o lib/src/stm32f10x_rtc.o lib/src/stm32f10x_sdio.o lib/src/stm32f10x_spi.o lib/src/stm32f10x_tim.o lib/src/stm32f10x_usart.o lib/src/stm32f10x_wwdg.o lib/src/system_stm32f10x.o

#ENC28J60=lib/enc28j60/enc28j60_driver.c

ENC28J60=lib/tcpip/enc28j60.o lib/tcpip/ip_arp_udp_tcp.o lib/tcpip/SPI.o #lib/tcpip/simple_server.o

SRC=src/main.o src/uart.o src/ws2812.o src/enc28j60.c src/write.o

CC=arm-none-eabi-gcc
CXX=arm-none-eabi-gcc
AS=arm-none-eabi-as
LD=arm-none-eabu-ld

INCLUDES=-I lib/inc -I lib/tcpip

SYS=MD
HSE=8000000

STARTUP=lib/startup/startup_stm32f10x_$(shell echo $(SYS) | tr A-Z a-z).o

CFLAGS=-mcpu=cortex-m3 -mthumb -mlittle-endian -mthumb-interwork -Wl,-T,lib/ld/stm32_flash.ld -DUSE_STDPERIPH_DRIVER -DSTM32F10X_$(SYS) -DHSE_VALUE=$(HSE) -lnosys -nostdlib -Os $(INCLUDES)

LDFLAGS=-lnosys -nostdlib -lc -lm

main.elf: $(STDLIB) $(SRC) $(ENC28J60) $(STARTUP)
	$(CC) $(SRC) $(STARTUP) $(ENC28J60) $(STDLIB) $(CFLAGS) -o main.elf

.PHONY: clean program

program: main.elf
	openocd -f "board/stm32f10x.cfg" -c "init"  -c "reset halt" -c "flash write_image erase main.elf" -c "reset run" -c "shutdown"


clean:
	find . -name "*.o" | xargs rm

