/********************************************* 
 * modified: 2007-08-08 
 * Author  : awake 
 * Copyright: GPL V2 
 * http://www.icdev.com.cn/?2213/ 
 * Host chip: ADUC7026 
 **********************************************/ 

#include "enc28j60.h" 

const unsigned char enc28j60_MAC[6] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06}; 

extern int simple_server(void); 

int ENC_main(void) 
{ 
	int rev = 0; 

	//    SAMDK_Init(); 

	//__enable_irq(); 

	simple_server(); 

	enc28j60Init((unsigned char *)enc28j60_MAC); 

	rev = enc28j60getrev(); 

	return rev; 
} 
