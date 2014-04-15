#ifndef __WRITE_H__
#define __WRITE_H__

#define BUFFER_SIZE (0x3840 + 42)

extern int32_t write_size;
extern uint8_t write_buffer[BUFFER_SIZE];

void request_write_dmx(void);
void request_write_ws2812(void);
void request_write_ws2801(void);

#endif /* __WRITE_H__ */

