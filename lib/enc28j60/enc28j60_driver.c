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

#include "enc28j60_driver.h"

/**
 * Writes an operation (or instruction) on the SPI line using the opcode and an
 * argument; the instruction is composed of three bits of opcode and 5 bits of 
 * argument.
 *
 * @param opcode	The operation code
 * @param arg		Operation's argument
 */
static void _write_op(uint8_t opcode, uint8_t arg)
{
	uint8_t n;
	n = arg & 0x1f;
	n |= (opcode << 5);
	e28j60_spi_rw(n);
}

/**
 * Reads from SPI.
 */
static uint8_t _spi_read(void) {
	return e28j60_spi_rw(0);
}

/**
 * Writes to SPI.
 *
 * @param b	Byte to write
 */
static void _spi_write(uint8_t b) {
	e28j60_spi_rw(b);
}

uint8_t e28j60_rcr(uint8_t reg)
{
	uint8_t value;
	
	e28j60_spi_cs_on();
	
	_write_op(E28J60_RCR_OP, reg);
	if (reg & E28J60_REG_TYPE_MASK) {
		_spi_read();
	}
	value = _spi_read();
	
	e28j60_spi_cs_off();
	
	return value;
}

void e28j60_rbm_cont(uint8_t* values, const uint8_t n)
{
	uint8_t i;
	
	e28j60_spi_cs_on();
	
	_write_op(E28J60_RBM_OP, E28J60_FUNNY_CONSTANT);
	for (i = 0; i < n; i++) {
		values[i] = _spi_read();
	}
	
	e28j60_spi_cs_off();
}

uint8_t e28j60_rbm(void)
{
	uint8_t value;
	
	e28j60_rbm_cont(&value, 1);
	
	return value;
}

void e28j60_wcr(uint8_t reg, uint8_t value)
{
	e28j60_spi_cs_on();
	
	_write_op(E28J60_WCR_OP, reg);
	_spi_write(value);
	
	e28j60_spi_cs_off();
}

void e28j60_wbm_cont(uint8_t* values, const uint8_t n)
{
	uint8_t i;
	
	e28j60_spi_cs_on();
	
	_write_op(E28J60_WBM_OP, E28J60_FUNNY_CONSTANT);
	for (i = 0; i < n; i++) {
		_spi_write(values[i]);
	}
	
	e28j60_spi_cs_off();
}

void e28j60_wbm(uint8_t value)
{
	e28j60_wbm_cont(&value, 1);
}

void e28j60_bfs(uint8_t reg, uint8_t or_mask)
{
	e28j60_spi_cs_on();
	
	_write_op(E28J60_BFS_OP, reg);
	_spi_write(or_mask);
	
	e28j60_spi_cs_off();
}

void e28j60_bfc(uint8_t reg, uint8_t inv_and_mask)
{
	e28j60_spi_cs_on();
	
	_write_op(E28J60_BFC_OP, reg);
	_spi_write(inv_and_mask);
	
	e28j60_spi_cs_off();
}

void e28j60_sc(void)
{
	e28j60_spi_cs_on();
	
	_write_op(E28J60_SC_OP, E28J60_SC_CONSTANT);
	
	e28j60_spi_cs_off();
}
