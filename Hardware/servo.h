/*
 * @file	servo.h
 * @auther	Tamap
 * @brief	Servo Driver
 * @date	2019-7-14
 * 
 * IO res:
 * T1_1 : PA8
 * T1_2 : PA9
 * T1_3 : PA10
 * T1_4 : PA11
 * T8_1 : PC6
 * T8_2 : PC7
 * T8_3 : PC8
 * T8_4 : PC9
 * 
 * Hardware res:
 * T1 : TIM1
 * T8 : TIM8
 * GPIOA GPIOC
*/
#ifndef _SERVO_H_
#define _SERVO_H_

//Inc
#include "stm32f10x.h"

//Var

//Fun
/*
 * @brief Init the defaults Servo interface.
 */
#define Servo_Config Servo_ConfigT8
/*
 * @brief Init the Servo interface at PC6~PC9 with TIM8.
 */
void Servo_ConfigT8(void);
/*
 * @brief Init the Servo interface at PA8~PA11 with TIM1.
 */
void Servo_ConfigT1(void);

/*
 * @brief Get/Set output of servo channel
 * 
 * @param [in] tim	*TIM_Typedef:
 * 	-TIM8
 * 	-TIM1
 * @param [in] ch	Channel of pwm output:
 * 	-1~4
 * @return The channel
 * @par	Example:
 * @code
 * 	Servo_Ch(TIM8,1) = 1500; // 1.5ms
 * @endcode
 */
#define Servo_Ch(tim,ch) (tim->CCR##ch)

#endif
