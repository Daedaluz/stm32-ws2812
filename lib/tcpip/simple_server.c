/*********************************************
 * vim:sw=8:ts=8:si:et
 * To use the above modeline in vim you must have "set modeline" in your .vimrc
 * Author: Guido Socher
 * Copyright: GPL V2
 * See http://www.gnu.org/licenses/gpl.html
 *
 * Ethernet remote device and sensor
 * UDP and HTTP interface 
        url looks like this http://baseurl/password/command
        or http://baseurl/password/
 *
 * Title: Microchip ENC28J60 Ethernet Interface Driver
 * Chip type           : ATMEGA88 with ENC28J60
 * Note: there is a version number in the text. Search for tuxgraphics
 *********************************************/


/*********************************************
 * modified: 2007-08-08
 * Author  : awake
 * Copyright: GPL V2
 * http://www.icdev.com.cn/?2213/
 * Host chip: ADUC7026
**********************************************/




#include <net.h>
#include <enc28j60.h>
#include <stdint.h>
#include <string.h>


#define PSTR(s) s

//extern void delay_ms(unsigned char ms);

// please modify the following two lines. mac and ip have to be unique
// in your local area network. You can not have the same numbers in
// two devices:
static uint8_t mymac[6] = {0x54,0x55,0x58,0x10,0x00,0x24};
static uint8_t myip[4] = {192,168,0,25};
// base url (you can put a DNS name instead of an IP addr. if you have
// a DNS server (baseurl must end in "/"):
static char baseurl[]="http://192.168.0.25/";
static uint16_t mywwwport =80; // listen port for tcp/www (max range 1-254)
// or on a different port:
//static char baseurl[]="http://10.0.0.24:88/";
//static uint16_t mywwwport =88; // listen port for tcp/www (max range 1-254)
//
static uint16_t myudpport =1200; // listen port for udp
// how did I get the mac addr? Translate the first 3 numbers into ascii is: TUX

#define BUFFER_SIZE 1500//400
static uint8_t buf[BUFFER_SIZE+1];

// the password string (only the first 5 char checked), (only a-z,0-9,_ characters):
static char password[]="123456"; // must not be longer than 9 char
const unsigned char WebSide[] = { 
	"<p><b><span lang=EN-US>A/D </span>ÎÂ¶È¼àÊÓ<span lang=EN-US>:(</span>·¶Î§<span\r\n" 
		"lang=EN-US>0~<st1:chmetcnv UnitName=\"¡ãC\" SourceValue=\"50\" HasSpace=\"False\"\r\n" 
		"Negative=\"False\" NumberType=\"1\" TCSC=\"0\" w:st=\"on\">50¡ãC</st1:chmetcnv>)</span></b></p>\r\n" 
		"\r\n" 
		"<table class=MsoNormalTable border=1 cellspacing=0 cellpadding=0 width=540\r\n" 
		" style='width:405.0pt;mso-cellspacing:0cm;background:red;border:outset 4.5pt;\r\n" 
		" mso-padding-alt:0cm 0cm 0cm 0cm'>\r\n" 
		" <tr style='mso-yfti-irow:0;mso-yfti-firstrow:yes;mso-yfti-lastrow:yes'>\r\n" 
		"  <td style='padding:0cm 0cm 0cm 0cm'>\r\n" 
		"  <table class=MsoNormalTable border=0 cellspacing=0 cellpadding=0\r\n" 
		"   style='mso-cellspacing:0cm;mso-padding-alt:0cm 0cm 0cm 0cm'>\r\n" 
		"   <tr style='mso-yfti-irow:0;mso-yfti-firstrow:yes;mso-yfti-lastrow:yes'>\r\n" 
		"    <td style='background:lime;padding:0cm 0cm 0cm 0cm'>\r\n" 
		"    <p class=MsoNormal><span lang=EN-US>&nbsp;</span></p>\r\n" 
		"    </td>\r\n" 
};
// 
uint8_t verify_password(char *str)
{
	// the first characters of the received string are
	// a simple password/cookie:
	if (strncmp(password,str,5)==0)
	{
		return(1);
	}
	return(0);
}

// takes a string of the form password/commandNumber and analyse it
// return values: -1 invalid password, otherwise command number
//                -2 no command given but password valid
int8_t analyse_get_url(char *str)
{
	uint8_t i=0;
	if (verify_password(str)==0)
	{
		return(-1);
	}
	// find first "/"
	// passw not longer than 9 char:
	while(*str && i<10 && *str >',' && *str<'{')
	{
		if (*str=='/')
		{
			str++;
			break;
		}
		i++;
		str++;
	}
	if (*str < 0x3a && *str > 0x2f)
	{
		// is a ASCII number, return it
		return(*str-0x30);
	}
	return(-2);
}

// prepare the webpage by writing the data to the tcp send buffer
uint16_t print_webpage(uint8_t *buf,uint8_t on_off)
{
	uint16_t plen;
	plen=fill_tcp_data_p(buf,0,PSTR("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\nPragma: no-cache\r\n\r\n"));
	plen=fill_tcp_data_p(buf,plen,PSTR("<center><p>LEDÊä³ö: "));
	if (on_off)
	{
		plen=fill_tcp_data_p(buf,plen,PSTR("<font color=\"#00FF00\"> ÁÁ</font>"));
	}
	else
	{
		plen=fill_tcp_data_p(buf,plen,PSTR("Ãð"));
	}
	plen=fill_tcp_data_p(buf,plen,PSTR(" <small><a href=\""));
	plen=fill_tcp_data(buf,plen,baseurl);
	plen=fill_tcp_data(buf,plen,password);
	plen=fill_tcp_data_p(buf,plen,PSTR("\">[Ë¢ÐÂ]</a></small></p>\n<p><a href=\""));
	// the url looks like this http://baseurl/password/command
	plen=fill_tcp_data(buf,plen,baseurl);
	plen=fill_tcp_data(buf,plen,password);
	if (on_off)
	{
		plen=fill_tcp_data_p(buf,plen,PSTR("/0\">¹Ø±ÕLED</a><p>"));
	}
	else
	{
		plen=fill_tcp_data_p(buf,plen,PSTR("/1\">¿ªÆôLED</a><p>"));
	}
	plen=fill_tcp_data_p(buf,plen,PSTR("</center><hr><br>STM32F103VBT6 »ùÓÚUCOS-II V2.85 WEB ÍøÒ³ÊÔÑé----ÑôÖ¾ÓÂ\n")); 
	//plen=fill_tcp_data_p(buf,plen,PSTR(WebSide)); 

	return(plen);
}

int simple_server(void)
{      
	uint16_t plen;
	uint16_t dat_p;
	uint8_t i=0;
	uint8_t cmd_pos=0;
	int8_t cmd;
	uint8_t payloadlen=0;
	char str[30];
	char cmdval;


	Del_1ms(100); 
	/*initialize enc28j60*/
	enc28j60Init(mymac); 
	init_ip_arp_udp_tcp(mymac,myip,mywwwport); 
	//Ö¸Ê¾µÆ×´Ì¬:0x476 is PHLCON LEDA(ÂÌ)=links status, LEDB(ºì)=receive/transmit 
	enc28j60PhyWrite(PHLCON,0x7a4);	 
	enc28j60clkout(2); // change clkout from 6.25MHz to 12.5MHz
	Del_1ms(20); 

	//init the ethernet/ip layer:
	while(1)
	{
//		OSTimeDlyHMSM(0, 0, 0, 50); 
                Del_1ms(50);
		// get the next new packet:
		plen = enc28j60PacketReceive(BUFFER_SIZE, buf); 
		//USART_DMASendData(USART1,buf,plen); 
		/*plen will ne unequal to zero if there is a valid packet (without crc error) */ 
		if(plen==0)
		{
                        cputs("ERROR\n");
			continue;
		}
		// arp is broadcast if unknown but a host may also
		// verify the mac address by sending it to 
		// a unicast address.
		if(eth_type_is_arp_and_my_ip(buf,plen))
		{
                        cputs("arp\n");
			make_arp_answer_from_request(buf);
			//USART_DMASendText(USART1,"make_arp_answer_from_request\n");
			continue;
		}

		// check if ip packets are for us:
		if(eth_type_is_ip_and_my_ip(buf,plen)==0)
		{
                        char buff[100];
                        sprintf(buff, "not for us (%d.%d.%d.%d)\n", buf[IP_DST_P+0],buf[IP_DST_P+1],buf[IP_DST_P+2],buf[IP_DST_P+3]);
                        cputs(buff);
			continue;
		}


		if(buf[IP_PROTO_P]==IP_PROTO_ICMP_V && buf[ICMP_TYPE_P]==ICMP_TYPE_ECHOREQUEST_V)
		{
			// a ping packet, let's send pong	
			make_echo_reply_from_request(buf, plen);
			//USART_DMASendText(USART1,"make_echo_reply_from_request\n");
			continue;
		}
		// tcp port www start, compare only the lower byte
		if (buf[IP_PROTO_P]==IP_PROTO_TCP_V&&buf[TCP_DST_PORT_H_P]==0&&buf[TCP_DST_PORT_L_P]==mywwwport)
		{
                        cputs("TCP!\n");
			if (buf[TCP_FLAGS_P] & TCP_FLAGS_SYN_V)
			{
				make_tcp_synack_from_syn(buf);
				// make_tcp_synack_from_syn does already send the syn,ack
				continue;
			}
			if (buf[TCP_FLAGS_P] & TCP_FLAGS_ACK_V)
			{
				init_len_info(buf); // init some data structures
				// we can possibly have no data, just ack:
				dat_p=get_tcp_data_pointer();
				if (dat_p==0)
				{
					if (buf[TCP_FLAGS_P] & TCP_FLAGS_FIN_V)
					{
						// finack, answer with ack
						make_tcp_ack_from_any(buf);
					}
					// just an ack with no data, wait for next packet
					continue;
				}
				if (strncmp("GET ",(char *)&(buf[dat_p]),4)!=0)
				{
					// head, post and other methods:
					//
					// for possible status codes see:
					// http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html
					plen=fill_tcp_data_p(buf,0,PSTR("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n<h1>200 OK</h1>"));
					goto SENDTCP;
				}
				if (strncmp("/ ",(char *)&(buf[dat_p+4]),2)==0)
				{
					plen=fill_tcp_data_p(buf,0,PSTR("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n"));
					plen=fill_tcp_data_p(buf,plen,PSTR("<p>Usage: "));
					plen=fill_tcp_data(buf,plen,baseurl);
					plen=fill_tcp_data_p(buf,plen,PSTR("password</p>"));
					goto SENDTCP;
				}
				cmd=analyse_get_url((char *)&(buf[dat_p+5]));
				// for possible status codes see:
				// http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html
				if (cmd==-1)
				{
					plen=fill_tcp_data_p(buf,0,PSTR("HTTP/1.0 401 Unauthorized\r\nContent-Type: text/html\r\n\r\n<h1>401 Unauthorized</h1>"));
					goto SENDTCP;
				}
				if (cmd==1)
				{
					//PORTD|= (1<<PD7);// transistor on
					//IOCLR |= (1<<26); 
//					LED1ON();
					i=1;
				}
				if (cmd==0)
				{
					//PORTD &= ~(1<<PD7);// transistor off
					//IOSET |= (1<<26); 
//					LED1OFF();
					i=0;
				}
				// if (cmd==-2) or any other value
				// just display the status:
				plen=print_webpage(buf,(i));

SENDTCP:
				make_tcp_ack_from_any(buf); // send ack for http get
				make_tcp_ack_with_data(buf,plen); // send data
				continue;
			}
		}
		// tcp port www end
		//
		// udp start, we listen on udp port 1200=0x4B0
		if (buf[IP_PROTO_P]==IP_PROTO_UDP_V&&buf[UDP_DST_PORT_H_P]==4&&buf[UDP_DST_PORT_L_P]==0xb0)
		{
                        cputs("Hello!\n");
			payloadlen=buf[UDP_LEN_L_P]-UDP_HEADER_LEN;
			// you must sent a string starting with v
			// e.g udpcom version 10.0.0.24
			if (verify_password((char *)&(buf[UDP_DATA_P])))
			{
				// find the first comma which indicates 
				// the start of a command:
				cmd_pos=0;
				while(cmd_pos<payloadlen)
				{
					cmd_pos++;
					if (buf[UDP_DATA_P+cmd_pos]==',')
					{
						cmd_pos++; // put on start of cmd
						break;
					}
				}
				// a command is one char and a value. At
				// least 3 characters long. It has an '=' on
				// position 2:
				if (cmd_pos<2 || cmd_pos>payloadlen-3 || buf[UDP_DATA_P+cmd_pos+1]!='=')
				{
					strcpy(str,"e=no_cmd\n");
					goto ANSWER;
				}
				// supported commands are
				// t=1 t=0 t=?
				if (buf[UDP_DATA_P+cmd_pos]=='t')
				{
					cmdval=buf[UDP_DATA_P+cmd_pos+2];
					if(cmdval=='1')
					{
						//PORTD|= (1<<PD7);// transistor on
						//IOCLR |= (1<<26); 
						//LED1ON();
						strcpy(str,"t=1\n");
						goto ANSWER;
					}
					else if(cmdval=='0')
					{
						//PORTD &= ~(1<<PD7);// transistor off
						//IOSET |= (1<<26); 
						//LED1OFF();
						strcpy(str,"t=0\n");
						goto ANSWER;
					}
					else if(cmdval=='?')
					{
						/* 
						   if (IOPIN & (1<<26))
						   {
						   strcpy(str,"t=1");
						   goto ANSWER;
						   } 
						   */
						strcpy(str,"t=0\n");
						goto ANSWER;
					}
				}
				strcpy(str,"e=no_such_cmd\n");
				goto ANSWER;
			}
			strcpy(str,"e=invalid_pw\n");
ANSWER:
                        cputs(str);
			make_udp_reply_from_request(buf,str,strlen(str),myudpport);

		}
	}
	//        return (0);
}
