/**
 * @file servo_cfg.c
 * @author Péter Dvorák (xdvorakp@stuba.sk)
 * @brief Source file for the servo motor driver
 * @version 0.1
 * @date 2022-12-06
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "servo.h"

const SERVO_CfgType SERVO_CfgParam[NUM_OF_SERVO_MOTORS] =
{
	// Servo Motor 1 Configurations
    {
	    GPIOA,
		GPIO_PIN_0,
		TIM2,
		&TIM2->CCR1,
		TIM_CHANNEL_1,
		72000000,
		0.65,
		2.3
	}
};