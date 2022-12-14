/**
 * @file ll_servo.c
 * @author Péter Dvorák (xdvorakp@stuba.sk)
 * @brief Source file for the servo motor driver
 * @version 0.1
 * @date 2022-12-07
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "ll_servo.h"

void Start_Timer(TIM_TypeDef *TIMx, uint32_t Channels)
{
	if (!LL_TIM_IsEnabledCounter(TIMx))
	{
		  /* Start Update Timer */
		LL_TIM_CC_EnableChannel(TIMx, Channels);
		LL_TIM_EnableCounter(TIMx);
		LL_TIM_EnableAllOutputs(TIMx);
	}
}

void servoMotorInit(ll_servo_t *servo)
{
    servo->angle_rng = (servo->angle_rng) ? servo->angle_rng : 180;
    uint16_t arr = LL_TIM_GetAutoReload(servo->slave_tim);

    if(servo->master_tim)
    {
        servo->angle_min = (servo->angle_min != ' ') ? servo->angle_min : (arr/2);
        servo->angle_max = (servo->angle_max != ' ') ? servo->angle_max : arr;
        Start_Timer(servo->master_tim, servo->channel);
    }
    else{
        servo->angle_min = (servo->angle_min != ' ') ? servo->angle_min : (arr / 20);
		servo->angle_max = (servo->angle_max != ' ') ? servo->angle_max : (arr / 10);
    }
    Start_Timer(servo->slave_tim, servo->channel);
}

void servoSetAngle(ll_servo_t *servo, uint8_t deg)
{
    deg = (deg > servo->angle_rng) ? servo->angle_rng : deg;
	deg = (deg < 0) ? 0 : deg;
	uint16_t diff = servo->angle_max - servo->angle_min;
	uint16_t duty = ((deg * diff) / servo->angle_rng) + servo->angle_min;
	duty = (duty < 1) ? 1 : duty;

    if (!LL_TIM_CC_IsEnabledChannel(servo->slave_tim, servo->channel))
	{
		LL_TIM_CC_EnableChannel(servo->slave_tim, servo->channel);
	}
	LL_TIM_OC_SetCompareCH1(servo->slave_tim, servo->duty);	// Replace if using other channel
}

void servoCalibrate(ll_servo_t *servo, uint16_t ccr)
{
    if(!LL_TIM_CC_EnableChannel(servo->slave_tim, servo->channel))
    {
        LL_TIM_CC_EnableChannel(servo->slave_tim, servo->channel);
    }
    LL_TIM_OC_SetCompareCH1(servo->slave_tim, servo->duty);
}
