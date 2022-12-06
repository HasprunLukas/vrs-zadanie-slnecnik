/**
 * @file servo.h
 * @author Péter Dvorák (xdvorakp@stuba.sk)
 * @brief Header file for the servo motor driver
 * @version 0.1
 * @date 2022-12-06
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __SERVO_DRIVER__
#define __SERVO_DRIVER__

#define NUM_OF_SERVO_MOTORS 1

//Servo_InitStruct
typedef struct
{
    GPIO_TypeDef * SERVO_GPIO;
    uint16_t       SERVO_PIN;
    TIM_TypeDef*   TIM_Instance;
    uint32_t*      TIM_CCRx;
    uint32_t       PWM_TIM_CH;
    uint32_t       TIM_CLK;
    float          MinPulse;
    float          MaxPulse;
}SERVO_CfgType;

/*Functions*/

void SERVO_Init(uint16_t au16_SERVO_instance);

void SERVO_MoveTo(uint16_t au16_SERVO_instance, float af_angle);

void SERVO_RawMove(uint16_t au16_SERVO_instance, uint16_t au16_pulse);

uint16_t SERVO_GetMaxPulse(uint16_t au16_SERVO_instance);

uint16_t SERVO_GetMinPulse(uint16_t au16_SERVO_instance);

void SERVO_Sweep(uint16_t au16_SERVO_instance);

#endif