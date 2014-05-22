/*********************************************
 * vim:sw=8:ts=8:si:et
 * To use the above modeline in vim you must have "set modeline" in your .vimrc
 * Author: Guido Socher 
 * Copyright: GPL V2
 *
 * Based on the net.h file from the AVRlib library by Pascal Stang.
 * For AVRlib See http://www.procyonengineering.com/
 * Used with explicit permission of Pascal Stang.
 *
 * Chip type           : ATMEGA88 with ENC28J60
 *********************************************/ 
 
 
 
/********************************************* 
 * modified: 2007-08-08 
 * Author  : awake 
 * Copyright: GPL V2 
 * http://www.icdev.com.cn/?2213/ 
 * Host chip: ADUC7026 
**********************************************/ 
 

// notation: _P = position of a field
//           _V = value of a field

//@{

#ifndef NET_H
#define NET_H
#include <stdint.h>


// ****** ARP ******

#define ARP_OP_REQ 0
#define ARP_OP_RESP 1

// ******* ETH *******
#define ETH_HEADER_LEN	14
// values of certain bytes:
#define ETHTYPE_ARP_H_V 0x08
#define ETHTYPE_ARP_L_V 0x06
#define ETHTYPE_IP_H_V  0x08
#define ETHTYPE_IP_L_V  0x00
// byte positions in the ethernet frame:
//
// Ethernet type field (2bytes):
#define ETH_TYPE_H_P 12
#define ETH_TYPE_L_P 13
//
#define ETH_DST_MAC 0
#define ETH_SRC_MAC 6


// ******* ARP *******
#define ETH_ARP_OPCODE_REPLY_H_V 0x0
#define ETH_ARP_OPCODE_REPLY_L_V 0x02
//
#define ETHTYPE_ARP_L_V 0x06
// arp.dst.ip
#define ETH_ARP_DST_IP_P 0x26
// arp.opcode
#define ETH_ARP_OPCODE_H_P 0x14
#define ETH_ARP_OPCODE_L_P 0x15
// arp.src.mac
#define ETH_ARP_SRC_MAC_P 0x16
#define ETH_ARP_SRC_IP_P 0x1c
#define ETH_ARP_DST_MAC_P 0x20
#define ETH_ARP_DST_IP_P 0x26

// ******* IP *******
#define IP_HEADER_LEN	20
// ip.src
#define IP_SRC_P 0x1a
#define IP_DST_P 0x1e
#define IP_HEADER_LEN_VER_P 0xe
#define IP_CHECKSUM_P 0x18
#define IP_TTL_P 0x16
#define IP_FLAGS_P 0x14
#define IP_P 0xe
#define IP_TOTLEN_H_P 0x10
#define IP_TOTLEN_L_P 0x11

#define IP_PROTO_P 0x17  

#define IP_PROTO_ICMP_V 1
#define IP_PROTO_TCP_V 6
// 17=0x11
#define IP_PROTO_UDP_V 17
// ******* ICMP *******
#define ICMP_TYPE_ECHOREPLY_V 0
#define ICMP_TYPE_ECHOREQUEST_V 8
//
#define ICMP_TYPE_P 0x22
#define ICMP_CHECKSUM_P 0x24

// ******* UDP *******
#define UDP_HEADER_LEN	8
//
#define UDP_SRC_PORT_H_P 0x22
#define UDP_SRC_PORT_L_P 0x23
#define UDP_DST_PORT_H_P 0x24
#define UDP_DST_PORT_L_P 0x25
//
#define UDP_LEN_H_P 0x26
#define UDP_LEN_L_P 0x27
#define UDP_CHECKSUM_H_P 0x28
#define UDP_CHECKSUM_L_P 0x29
#define UDP_DATA_P 0x2a

// ******* TCP *******
#define TCP_SRC_PORT_H_P 0x22
#define TCP_SRC_PORT_L_P 0x23
#define TCP_DST_PORT_H_P 0x24
#define TCP_DST_PORT_L_P 0x25
// the tcp seq number is 4 bytes 0x26-0x29
#define TCP_SEQ_H_P 0x26
#define TCP_SEQACK_H_P 0x2a
// flags: SYN=2
#define TCP_FLAGS_P 0x2f
#define TCP_FLAGS_SYN_V 2
#define TCP_FLAGS_FIN_V 1
#define TCP_FLAGS_PUSH_V 8
#define TCP_FLAGS_SYNACK_V 0x12
#define TCP_FLAGS_ACK_V 0x10
#define TCP_FLAGS_PSHACK_V 0x18
//  plain len without the options:
#define TCP_HEADER_LEN_PLAIN 20
#define TCP_HEADER_LEN_P 0x2e
#define TCP_CHECKSUM_H_P 0x32
#define TCP_CHECKSUM_L_P 0x33
#define TCP_OPTIONS_P 0x36
//

// ****** DHCP *****

#define DHCP_OP_REQUEST 1
#define DHCP_OP_REPLY 2

#define DHCP_OP_P (ETH_HEADER_LEN+IP_HEADER_LEN+UDP_HEADER_LEN)
#define DHCP_HTYPE_P (DHCP_OP_P+1)
#define DHCP_HLEN_P (DHCP_OP_P+2)
#define DHCP_HOPS_P (DHCP_OP_P+3)

#define DHCP_XID1_P (DHCP_OP_P+4)
#define DHCP_XID2_P (DHCP_OP_P+5)
#define DHCP_XID3_P (DHCP_OP_P+6)
#define DHCP_XID4_P (DHCP_OP_P+7)

#define DHCP_SECS_H_P (DHCP_OP_P+8)
#define DHCP_SECS_L_P (DHCP_OP_P+9)

#define DHCP_FLAGS_H_P (DHCP_OP_P+10)
#define DHCP_FLAGS_L_P (DHCP_OP_P+11)

#define DHCP_CIADDR1_P (DHCP_OP_P+12)
#define DHCP_CIADDR2_P (DHCP_OP_P+13)
#define DHCP_CIADDR3_P (DHCP_OP_P+14)
#define DHCP_CIADDR4_P (DHCP_OP_P+15)

#define DHCP_YIADDR1_P (DHCP_OP_P+16)
#define DHCP_YIADDR2_P (DHCP_OP_P+17)
#define DHCP_YIADDR3_P (DHCP_OP_P+18)
#define DHCP_YIADDR4_P (DHCP_OP_P+19)

#define DHCP_SIADDR1_P (DHCP_OP_P+20)
#define DHCP_SIADDR2_P (DHCP_OP_P+21)
#define DHCP_SIADDR3_P (DHCP_OP_P+22)
#define DHCP_SIADDR4_P (DHCP_OP_P+23)

#define DHCP_GIADDR1_P (DHCP_OP_P+20)
#define DHCP_GIADDR2_P (DHCP_OP_P+21)
#define DHCP_GIADDR3_P (DHCP_OP_P+22)
#define DHCP_GIADDR4_P (DHCP_OP_P+23)
#endif
