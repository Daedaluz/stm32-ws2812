void usleep(uint32_t us) {
	uint32_t l;
	uint32_t x;
	for(x = 0; x < us; x++)
		for(l = 0; l < 72000000; l++);
}

void msleep(uint32_t msec) {
	uint32_t x;
	for(x = 0; x < msec; x++)
		usleep(1000);
}

void sleep(uint32_t sec){
	uint32_t x;
	for(x = 0; x < sec; x++)
		msleep(1000);
}

