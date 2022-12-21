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
	vd6283tx_set_gpio1();
	LL_mDelay(10);
	vd6283tx_sda_config();
	LL_mDelay(10);
	vd6283tx_als_channel_enable();
	LL_mDelay(10);
	vd6283tx_als_channel6_enable;
	LL_mDelay(10);

	return status;
}

void vd6283tx_set_gpio1(void)
{
	volatile uint8_t config = vd6283tx_read_byte(VD6283TX_GPIO1_DRV_CFG);
	config &= ~0x02;
	config |= 0x01;
	vd6283tx_write_byte(VD6283TX_GPIO1_DRV_CFG,config);
}

void vd6283tx_sda_config(void)
{
	volatile uint8_t config = vd6283tx_read_byte(VD6283TX_SDA_DRV_CFG);
	config &= ~0x0F;
	config |= 0x09;
	vd6283tx_write_byte(VD6283TX_SDA_DRV_CFG,config);
}

void vd6283tx_als_channel_enable(void)
{
	volatile uint8_t als_enable = vd6283tx_read_byte(VD6283TX_ALS_CHANNEL_ENABLE);
	als_enable &= ~0x1F;
	als_enable |= 0x1F;
	vd6283tx_write_byte(VD6283TX_ALS_CHANNEL_ENABLE,als_enable);
}

void vd6283tx_als_channel6_enable(void)
{
	volatile uint8_t als_enable = vd6283tx_read_byte(VD6283TX_ALS_CHANNEL6_ENABLE);
	als_enable &= ~0x01;
	als_enable |= 0x01;
	LL_mDelay(100);
	vd6283tx_write_byte(VD6283TX_ALS_CHANNEL6_ENABLE,als_enable);
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
