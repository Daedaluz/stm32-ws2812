/*
libenc28j60d - ENC28J60 low-level driver for AVR MCUs
Copyright (C) 2011  Simon Marchi
                    Philippe Proulx

This library is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this library; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
#ifndef _ENC28J60DEFS_H
#define _ENC28J60DEFS_H

/*
 * SPI instructions definitions
 */

#define E28J60_RCR_OP 0
#define E28J60_RBM_OP 1
#define E28J60_WCR_OP 2
#define E28J60_WBM_OP 3
#define E28J60_BFS_OP 4
#define E28J60_BFC_OP 5
#define E28J60_SC_OP 6

#define E28J60_FUNNY_CONSTANT	0x1A
#define E28J60_SC_CONSTANT	0x1F

/*
 * Register definitions
 */

/*
 * Control registers encoding:
 * tbbaaaaa
 * t : 1 if it is a MAC or MII register, 0 otherwise
 * bb : bank where the register is found
 * aaaaa : address of the register
 */

#define E28J60_REG_TYPE_SHIFT 7
#define E28J60_REG_TYPE_MASK (1 << E28J60_REG_TYPE_SHIFT)
#define E28J60_REG_BANK_SHIFT 5
 
/* General */
#define E28J60_EIE	0x1B // 0 00 11011
#define E28J60_EIR	0x1C // 0 00 11100
#define E28J60_ESTAT	0x1D // 0 00 11101
#define E28J60_ECON2	0x1E // 0 00 11110
#define E28J60_ECON1	0x1F // 0 00 11111

/* Bank 0 */
#define E28J60_ERDPTL	0x00 // 0 00 00000
#define E28J60_ERDPTH	0x01 // 0 00 00001
#define E28J60_EWRPTL	0x02 // 0 00 00010
#define E28J60_EWRPTH	0x03 // 0 00 00011
#define E28J60_ETXSTL	0x04 // 0 00 00100
#define E28J60_ETXSTH	0x05 // 0 00 00101
#define E28J60_ETXNDL	0x06 // 0 00 00110
#define E28J60_ETXNDH	0x07 // 0 00 00111
#define E28J60_ERXSTL	0x08 // 0 00 01000
#define E28J60_ERXSTH	0x09 // 0 00 01001
#define E28J60_ERXNDL	0x0A // 0 00 01010
#define E28J60_ERXNDH	0x0B // 0 00 01011
#define E28J60_ERXRDPTL	0x0C // 0 00 01100
#define E28J60_ERXRDPTH	0x0D // 0 00 01101
#define E28J60_ERXWRPTL	0x0E // 0 00 01110
#define E28J60_ERXWRPTH	0x0F // 0 00 01111
#define E28J60_EDMASTL	0x10 // 0 00 10000
#define E28J60_EDMASTH	0x11 // 0 00 10001
#define E28J60_EDMANDL	0x12 // 0 00 10010
#define E28J60_EDMANDH	0x13 // 0 00 10011
#define E28J60_EDMADSTL	0x14 // 0 00 10100
#define E28J60_EDMADSTH	0x15 // 0 00 10101
#define E28J60_EDMACSL	0x16 // 0 00 10110
#define E28J60_EDMACSH	0x17 // 0 00 10111

/* Bank 1 */
#define E28J60_EHT0	0x20 // 0 01 00000
#define E28J60_EHT1	0x21 // 0 01 00001
#define E28J60_EHT2	0x22 // 0 01 00010
#define E28J60_EHT3	0x23 // 0 01 00011
#define E28J60_EHT4	0x24 // 0 01 00100
#define E28J60_EHT5	0x25 // 0 01 00101
#define E28J60_EHT6	0x26 // 0 01 00110
#define E28J60_EHT7	0x27 // 0 01 00111
#define E28J60_EPMM0	0x28 // 0 01 01000
#define E28J60_EPMM1	0x29 // 0 01 01001
#define E28J60_EPMM2	0x2A // 0 01 01010
#define E28J60_EPMM3	0x2B // 0 01 01011
#define E28J60_EPMM4	0x2C // 0 01 01100
#define E28J60_EPMM5	0x2D // 0 01 01101
#define E28J60_EPMM6	0x2E // 0 01 01110
#define E28J60_EPMM7	0x2F // 0 01 01111
#define E28J60_EPMCSL	0x30 // 0 01 10000
#define E28J60_EPMCSH	0x31 // 0 01 10001
#define E28J60_EPMOL	0x34 // 0 01 10100
#define E28J60_EPMOH	0x35 // 0 01 10101
#define E28J60_EWOLIE	0x36 // 0 01 10110
#define E28J60_EWOLIR	0x37 // 0 01 10111
#define E28J60_ERXFCON	0x38 // 0 01 11000
#define E28J60_EPKTCNT	0x39 // 0 01 11001

/* Bank 2 */
#define E28J60_MACON1	0xC0 // 1 10 00000
#define E28J60_MACON2	0xC1 // 1 10 00001
#define E28J60_MACON3	0xC2 // 1 10 00010
#define E28J60_MACON4	0xC3 // 1 10 00011
#define E28J60_MABBIPG	0xC4 // 1 10 00100
#define E28J60_MAIPGL	0xC6 // 1 10 00110
#define E28J60_MAIPGH	0xC7 // 1 10 00111
#define E28J60_MACLCON1	0xC8 // 1 10 01000
#define E28J60_MACLCON2	0xC9 // 1 10 01001
#define E28J60_MAMXFLL	0xCA // 1 10 01010
#define E28J60_MAMXFLH	0xCB // 1 10 01011
#define E28J60_MAPHSUP	0xCD // 1 10 01101
#define E28J60_MICON	0xD1 // 1 10 10001
#define E28J60_MICMD	0xD2 // 1 10 10010
#define E28J60_MIREGADR	0xD4 // 1 10 10100
#define E28J60_MIWRL	0xD6 // 1 10 10110
#define E28J60_MIWRH	0xD7 // 1 10 10111
#define E28J60_MIRDL	0xD8 // 1 10 11000
#define E28J60_MIRDH	0xD9 // 1 10 11001

/* Bank 3 */
#define E28J60_MAADR1	0xE0 // 1 11 00000
#define E28J60_MAADR0	0xE1 // 1 11 00001
#define E28J60_MAADR3	0xE2 // 1 11 00010
#define E28J60_MAADR2	0xE3 // 1 11 00011
#define E28J60_MAADR5	0xE4 // 1 11 00100
#define E28J60_MAADR4	0xE5 // 1 11 00101
#define E28J60_EBSTSD	0x66 // 0 11 00110
#define E28J60_EBSTCON	0x67 // 0 11 00111
#define E28J60_EBSTCSL	0x68 // 0 11 01000
#define E28J60_EBSTCSH	0x69 // 0 11 01001
#define E28J60_MISTAT	0xEA // 1 11 01010
#define E28J60_EREVID	0x72 // 0 11 10010
#define E28J60_ECOCON	0x75 // 0 11 10101
#define E28J60_EFLOCON	0x77 // 0 11 10111
#define E28J60_EPAUSL	0x78 // 0 11 11000
#define E28J60_EPAUSH	0x79 // 0 11 11001

/*
 * PHY registers
 */
#define E28J60_PHCON1	0x00
#define E28J60_PHSTAT1	0x01
#define E28J60_PHID1	0x02
#define E28J60_PHID2	0x03
#define E28J60_PHCON2	0x10
#define E28J60_PHSTAT2	0x11
#define E28J60_PHIE	0x12
#define E28J60_PHIR	0x13
#define E28J60_PHLCON	0x14

/*
 * Bit definitions
 */ 

/* EIE */
#define E28J60_INTIE	7
#define E28J60_PKTIE	6
#define E28J60_DMAIE	5
#define E28J60_LINKIE	4
#define E28J60_TXIE	3
#define E28J60_WOLIE	2
#define E28J60_TXERIE	1
#define E28J60_RXERIE	0

/* EIR */
#define E28J60_PKTIF	6
#define E28J60_DMAIF	5
#define E28J60_LINKIF	4
#define E28J60_TXIF	3
#define E28J60_WOLIF	2
#define E28J60_TXERIF	1
#define E28J60_RXERIF	0

/* ESTAT */
#define E28J60_INT	7
#define E28J60_LATECOL	4
#define E28J60_RXBUSY	2
#define E28J60_TXABRT	1
#define E28J60_CLKRDY	0

/* ECON2 */
#define E28J60_AUTOINC	7
#define E28J60_PKTDEC	6
#define E28J60_PWRSV	5
#define E28J60_VRPS	3

/* ECON1 */
#define E28J60_TXRST	7
#define E28J60_RXRST	6
#define E28J60_DMAST	5
#define E28J60_CSUMEN	4
#define E28J60_TXRTS	3
#define E28J60_RXEN	2
#define E28J60_BSEL1	1
#define E28J60_BSEL0	0

/* EWOLIE */
#define E28J60_UCWOLIE	7
#define E28J60_AWOLIE	6
#define E28J60_PMWOLIE	4
#define E28J60_MPWOLIE	3
#define E28J60_HTWOLIE	2
#define E28J60_MCWOLIE	1
#define E28J60_BCWOLIE	0

/* EWOLIR */
#define E28J60_UCWOLIF	7
#define E28J60_AWOLIF	6
#define E28J60_PMWOLIF	4
#define E28J60_MPWOLIF	3
#define E28J60_HTWOLIF	2
#define E28J60_MCWOLIF	1
#define E28J60_BCWOLIF	0

/* ERXFCON */
#define E28J60_UCEN	7
#define E28J60_ANDOR	6
#define E28J60_CRCEN	5
#define E28J60_PMEN	4
#define E28J60_MPEN	3
#define E28J60_HTEN	2
#define E28J60_MCEN	1
#define E28J60_BCEN	0

/* MACON1 */
#define E28J60_LOOPBK	4
#define E28J60_TXPAUS	3
#define E28J60_RXPAUS	2
#define E28J60_PASSALL	1
#define E28J60_MARXEN	0

/* MACON2 */
#define E28J60_MARST	7
#define E28J60_RNDRST	6
#define E28J60_MARXRST	3
#define E28J60_RFUNRST	2
#define E28J60_MATXRST	1
#define E28J60_TFUNRST	0

/* MACON3 */
#define E28J60_PADCFG2	7
#define E28J60_PADCFG1	6
#define E28J60_PADCFG0	5
#define E28J60_TXCRCEN	4
#define E28J60_PHDRLEN	3
#define E28J60_HFRMEN	2
#define E28J60_FRMLNEN	1
#define E28J60_FULDPX	0

/* MACON4 */
#define E28J60_DEFER	6
#define E28J60_BPEN	5
#define E28J60_NOBKOFF	4
#define E28J60_LONGPRE	1
#define E28J60_PUREPRE	0

/* MAPHSUP */
#define E28J60_RSTINTFC	7
#define E28J60_RSTRMII	3

/* MICON */
#define E28J60_RSTMII	7

/* MICMD */
#define E28J60_MIISCAN	1
#define E28J60_MIIRD	0

/* EBSTCON */
#define E28J60_PSV2	7
#define E28J60_PSV1	6
#define E28J60_PSV0	5
#define E28J60_PSEL	4
#define E28J60_TMSEL1	3
#define E28J60_TMSEL0	2
#define E28J60_TME	1
#define E28J60_BISTST	0

/* MISTAT */
#define E28J60_NVALID	2
#define E28J60_SCAN	1
#define E28J60_BUSY	0

/* ECOCON */
#define E28J60_COCON2	2
#define E28J60_COCON1	1
#define E28J60_COCON0	0

/* EFLOCON */
#define E28J60_FULDPXS	2
#define E28J60_FCEN1	1
#define E28J60_FCEN0	0

/* PHCON1 */
#define E28J60_PRST	15
#define E28J60_PLOOPBK	14
#define E28J60_PPWRSV	11
#define E28J60_PDPXMD	8

/* PHSTAT1 */
#define E28J60_PFDPX	12
#define E28J60_PHDPX	11
#define E28J60_LLSTAT	2
#define E28J60_JBSTAT	1

/* PHCON2 */
#define E28J60_FRCLNK	14
#define E28J60_TXDIS	13
#define E28J60_JABBER	10
#define E28J60_HDLDIS	8

/* PHSTAT2 */
#define E28J60_TXSTAT	13
#define E28J60_RXSTAT	12
#define E28J60_COLSTAT	11
#define E28J60_LSTAT	10
#define E28J60_DPXSTAT	9
#define E28J60_PLRITY	4

/* PHIE */
#define E28J60_PLNKIE	4
#define E28J60_PGEIE	1

/* PHIR */
#define E28J60_PLNKIF	4
#define E28J60_PGIF	2

/* PHLCON */
#define E28J60_LACFG3	11
#define E28J60_LACFG2	10
#define E28J60_LACFG1	9
#define E28J60_LACFG0	8
#define E28J60_LBCFG3	7
#define E28J60_LBCFG2	6
#define E28J60_LBCFG1	5
#define E28J60_LBCFG0	4
#define E28J60_LFRQ1	3
#define E28J60_LFRQ0	2
#define E28J60_STRCH	1

#endif /* _ENC28J60DEFS_H */
