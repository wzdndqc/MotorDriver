/*
 * @file	motor.h
 * @auther	Tamap
 * @brief	DC Motor Driver
 * @date	2019-7-16
 * 
 * IO res:
 * 
 * M1_PWM: PA8
 * M1_EA : PA15
 * M1_EB : PB3
 * M1_N1 : PC12
 * M1_N2 : PD2
 * 
 * M2_PWM: PA9
 * M2_EA : PA6
 * M2_EB : PA7
 * M2_N1 : PB4
 * M2_N2 : PB5
 * 
 * M3_PWM: PA10
 * M3_EA : PB6
 * M3_EB : PB7
 * M3_N1 : PC0
 * M3_N2 : PC1
 * 
 * M4_PWM: PA11
 * M4_EA : PA0
 * M4_EB : PA1
 * M4_N1 : PC2
 * M4_N2 : PC3
 * 
 * Hardware res:
 * 
 * PWM_OUT : TIM1_CH1~CH4
 * 
 * ENCODE1 : TIM2_CH1~CH2
 * ENCODE2 : TIM3_CH1~CH2
 * ENCODE3 : TIM4_CH1~CH2
 * ENCODE4 : TIM5_CH1~CH2
 * 
 * GPIOA GPIOB GPIOC GPIOD
 */
#ifndef _MOTOR_H_
#define _MOTOR_H_

//Inc
#include "stm32f10x.h"

//Def
#define GPIO_M1_PWM GPIOA
#define  PIN_M1_PWM GPIO_Pin_8
#define GPIO_M1_EA GPIOA
#define  PIN_M1_EA GPIO_Pin_15
#define GPIO_M1_EB GPIOB
#define  PIN_M1_EB GPIO_Pin_3
#define GPIO_M1_N1 GPIOC
#define  PIN_M1_N1 GPIO_Pin_12
#define GPIO_M1_N2 GPIOD
#define  PIN_M1_N2 GPIO_Pin_2
#define  TIM_M1_ENCODE TIM2

#define GPIO_M2_PWM GPIOA
#define  PIN_M2_PWM GPIO_Pin_9
#define GPIO_M2_EA GPIOA
#define  PIN_M2_EA GPIO_Pin_6
#define GPIO_M2_EB GPIOA
#define  PIN_M2_EB GPIO_Pin_7
#define GPIO_M2_N1 GPIOB
#define  PIN_M2_N1 GPIO_Pin_4
#define GPIO_M2_N2 GPIOB
#define  PIN_M2_N2 GPIO_Pin_5
#define  TIM_M2_ENCODE TIM3

#define GPIO_M3_PWM GPIOA
#define  PIN_M3_PWM GPIO_Pin_10
#define GPIO_M3_EA GPIOB
#define  PIN_M3_EA GPIO_Pin_6
#define GPIO_M3_EB GPIOB
#define  PIN_M3_EB GPIO_Pin_7
#define GPIO_M3_N1 GPIOC
#define  PIN_M3_N1 GPIO_Pin_0
#define GPIO_M3_N2 GPIOC
#define  PIN_M3_N2 GPIO_Pin_1
#define  TIM_M3_ENCODE TIM4

#define GPIO_M4_PWM GPIOA
#define  PIN_M4_PWM GPIO_Pin_11
#define GPIO_M4_EA GPIOA
#define  PIN_M4_EA GPIO_Pin_0
#define GPIO_M4_EB GPIOA
#define  PIN_M4_EB GPIO_Pin_1
#define GPIO_M4_N1 GPIOC
#define  PIN_M4_N1 GPIO_Pin_2
#define GPIO_M4_N2 GPIOC
#define  PIN_M4_N2 GPIO_Pin_3
#define  TIM_M4_ENCODE TIM5

//Var
/* 
 * @brief	Period(ms) of pwm output to motor
 */
#define MOTOR_PERIOD 20000

//Fun
/* 
 * @brief	Init TIM1~5 and GPIO for TB6612FNG driver
 */
void Motor_Config(void);
/* 
 * @brief	Init TIM2~5 and GPIO for Encode of motor.
 */
void Motor_Encoder_Config(void);
/*
 * @brief	Set motor state to Stop:OFF(High impedance)
 * 
 * @param[in] index	Index of motor:
 * 	- 1~4
 */
void Motor_Stop(uint8_t index);
/*
 * @brief	Set duty cycle of pwm output
 * 
 * @param[in] index	Index of motor:
 * 	- 1~4
 * @param[in] pwm	Duty cycle of pwm output, negative means reverse.
 */
void Motor_Output(uint8_t index,int16_t pwm);
/*
 * @brief Get step number of encoder form TIM2/3/4/5
 * 
 * @param[in] index	Index of motor:
 * 	- 1~4
 * @retval step number of encoder
 * @par	Example:
 * @code
 * 	step = Motor_GetStep(1);
 * @endcode
 */
uint16_t Motor_GetStep(uint8_t index);

#endif
