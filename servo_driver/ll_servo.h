/**
 * @file ll_servo.h
 * @author Péter Dvorák (xdvorakp@stuba.sk)
 * @brief Source file for the servo motor driver
 * @version 0.1
 * @date 2022-12-07
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __LL_SERVO_H__
#define __LL_SERVO_H__

#include "main.h"

typedef struct
{
    TIM_TypeDef *master_tim;
    TIM_TypeDef *slave_tim;
    uint32_t channel;
    uint32_t duty;
    uint16_t angle_min;
    uint16_t angle_max;
    uint8_t angle_rng;

} ll_servo_t;

void servoMotorInit(ll_servo_t *servo);
void servoSetAngle(ll_servo_t *servo, uint8_t deg);
void servoCalibrate(ll_servo_t *servo, uint16_t ccr);

#endif