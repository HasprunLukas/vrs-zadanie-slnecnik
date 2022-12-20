/*
 * lps25hb.h
 *
 *  Created on: 07. 11. 202
 *      Author: LukasHasprun
 */

#include "main.h"
#include "i2c.h"


#define VD6283TX_DEVICE_ID 				0x00
#define VD6283TX_DEVICE_ID_VALUE 		0x70
#define VD6283TX_SLAVE_ADDR 			0x40
#define VD6283TX_GPIO1_DRV_CFG			0x41
#define VD6283TX_SDA_DRV_CFG			0x3C
#define VD6283TX_ALS_CHANNEL_ENABLE		0x2E
#define VD6283TX_ALS_CH2_DATA_H		 	0x0A
#define VD6283TX_ALS_CH2_DATA_M		 	0x0B
#define VD6283TX_ALS_CH2_DATA_L		 	0x0C
#define VD6283TX_ALS_CTRL		 		0x03
#define VD6283TX_ALS_PERIOD		 		0x04
#define VD6283TX_ALS_CHANNEL6_ENABLE	0x2D
#define VD6283TX_ALS_CHANNEL_ENABLE		0x2E

uint8_t vd6283tx_init(void);
uint8_t vd6283tx_reinit(void);
uint8_t vd6283tx_read_byte(uint8_t reg_addr);
void vd6283tx_set_gpio1(void);
void vd6283tx_write_byte(uint8_t reg_addr, uint8_t value);
void vd6283tx_sda_config(void);
void vd6283tx_als_channel_enable(void);
int vd6283tx_get_als_ch2();
void vd6283tx_ctrl_reg_init(uint8_t config, uint8_t period);
void vd6283tx_ctrl_reg_reset(void);
void vd6283tx_als_channel_enable(void);
void vd6283tx_als_channel6_enable(void);
