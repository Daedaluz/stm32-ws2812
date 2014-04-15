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
#ifndef _ENC28J60_DRIVER_H
#define _ENC28J60_DRIVER_H

#include "enc28j60defs.h"

#include <stdint.h>

/**
 * Disables the SPI CS line (to be defined by user).
 */
extern void e28j60_spi_cs_off(void);

/**
 * Enables the SPI CS line (to be defined by user).
 */
extern void e28j60_spi_cs_on(void);

/**
 * Performs an SPI read/write (to be defined by user).
 *
 * @param c	Byte to send
 * @return	Byte read
 */
extern uint8_t e28j60_spi_rw(const uint8_t c);

/**
 * Reads the content of a control register; this function takes care of 
 * triggering the chip select signal, but does not switch the register banks.
 *
 * @param reg	Register to read
 * @return	Register value
 */
uint8_t e28j60_rcr(uint8_t reg);

/**
 * Reads n bytes of the buffer memory in the values array, starting at the 
 * current value of the ERDPT value.
 *
 * @param values	Array of at least n bytes to be filled
 * @param n		Number of bytes to read
 */
void e28j60_rbm_cont(uint8_t* values, const uint8_t n);

/**
 * Reads 1 byte of the buffer memory starting at the current value of the ERDPT
 * pointer.
 *
 * @return	The byte pointed to by the ERDPT pointer
 */
uint8_t e28j60_rbm(void);

/**
 * Writes to a register.
 *
 * @param reg		Register
 * @param value		Value
 */
void e28j60_wcr(uint8_t reg, uint8_t value);

/**
 * Writes n bytes from the values array to the buffer memory, starting at the 
 * current value of the EWRPT value.
 *
 * @param values	Array of at least n bytes
 * @param n		Number of bytes to write
 */
void e28j60_wbm_cont(uint8_t* values, const uint8_t n);

/**
 * Writes 1 byte of the buffer memory starting at the current value of the EWRPT
 * pointer.
 *
 * @return	Byte pointed to by the ERDPT pointer
 */
void e28j60_wbm(uint8_t value);

/**
 * Sets some bits in a control register (reg <= reg | or_mask).
 *
 * @param reg		Register to work with
 * @param or_mask	Bit mask to OR with the current register value
 */
void e28j60_bfs(uint8_t reg, uint8_t or_mask);

/**
 * Clears some bits in a control register (reg <= reg | ~inv_and_mask).
 *
 * @param reg		Register to work with.
 * @param inv_and_mask	Bit mask complement to AND with the current reg. value
 */
void e28j60_bfc(uint8_t reg, uint8_t inv_and_mask);

/**
 * System command (soft reset).
 */ 
void e28j60_sc(void);

#endif /* _ENC28J60_DRIVER_H */
