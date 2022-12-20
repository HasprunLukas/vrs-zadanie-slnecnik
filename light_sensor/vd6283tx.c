/*
 * lps25hb.c
 *
 *  Created on: 07. 11. 2022
 *      Author: LukasHasprun
 */

#include "vd6283tx.h"

uint8_t addres = VD6283TX_SLAVE_ADDR;

uint8_t vd6283tx_read_byte(uint8_t reg_addr)
{
	uint8_t data = 0;
	return *(i2c_master_read(&data, 1, reg_addr, addres, 0));
}

void vd6283tx_write_byte(uint8_t reg_addr, uint8_t value)
{
	i2c_master_write(value, reg_addr, addres, 0);
}

uint8_t vd6283tx_init(void)
{
	uint8_t status = 1;

	LL_mDelay(100);

	uint8_t val = vd6283tx_read_byte(VD6283TX_DEVICE_ID);

	if(val == VD6283TX_DEVICE_ID_VALUE)
	{
		status = 1;
	}
	else
	{
		status = 0;
	}


	vd6283tx_ctrl_reg_reset();
	LL_mDelay(10);
	vd6283tx_ctrl_reg_init(0x07, 0x01);
	LL_mDelay(10);

	return status;
}

int vd6283tx_get_als_ch2()
{
	volatile uint8_t ALS_CH2_DATA_H = vd6283tx_read_byte(VD6283TX_ALS_CH2_DATA_H);
	volatile uint8_t ALS_CH2_DATA_M = vd6283tx_read_byte(VD6283TX_ALS_CH2_DATA_M);
	volatile uint8_t ALS_CH2_DATA_L = vd6283tx_read_byte(VD6283TX_ALS_CH2_DATA_L);
	volatile int32_t ALS_CH2_DATA = ALS_CH2_DATA_H << 16 | ALS_CH2_DATA_M << 8 | ALS_CH2_DATA_L;
	return (int) ALS_CH2_DATA;
}

void vd6283tx_ctrl_reg_reset(void)
{
	volatile uint8_t als_ctrl = vd6283tx_read_byte(VD6283TX_ALS_CTRL);
	als_ctrl &= ~0x07;
	als_ctrl |= 0x00;

	LL_mDelay(100);
	vd6283tx_write_byte(VD6283TX_ALS_CTRL,als_ctrl);
	LL_mDelay(100);
}

void vd6283tx_ctrl_reg_init(uint8_t config, uint8_t period)
{
	volatile uint8_t als_ctrl = vd6283tx_read_byte(VD6283TX_ALS_CTRL);
	als_ctrl &= ~0x07;
	als_ctrl |= config;

	vd6283tx_write_byte(VD6283TX_ALS_CTRL,als_ctrl);

	LL_mDelay(100);

	volatile uint8_t als_period = vd6283tx_read_byte(VD6283TX_ALS_PERIOD);
	als_period &= ~0xFC;
	als_period |= period;

	vd6283tx_write_byte(VD6283TX_ALS_PERIOD,als_period);
}
