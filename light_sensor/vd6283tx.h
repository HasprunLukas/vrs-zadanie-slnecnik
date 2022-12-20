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
#define VD6283TX_ALS_CH2_DATA_H		 	0x0A
#define VD6283TX_ALS_CH2_DATA_M		 	0x0B
#define VD6283TX_ALS_CH2_DATA_L		 	0x0C
#define VD6283TX_ALS_CTRL		 		0x03
#define VD6283TX_ALS_PERIOD		 		0x04

uint8_t vd6283tx_init(void);
uint8_t vd6283tx_read_byte(uint8_t reg_addr);
void vd6283tx_write_byte(uint8_t reg_addr, uint8_t value);
int vd6283tx_get_als_ch2();
void vd6283tx_ctrl_reg_init(uint8_t config, uint8_t period);
void vd6283tx_ctrl_reg_reset(void);
