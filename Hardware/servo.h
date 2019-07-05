/*
Servo Driver

Hardware res:

T1_1 : PA8
T1_2 : PA9
T1_3 : PA10
T1_4 : PA11
T8_1 : PC6
T8_2 : PC7
T8_3 : PC8
T8_4 : PC9

Software res:

T1 : TIM1
T8 : TIM8

GPIOA GPIOC
*/
#ifndef _SERVO_H_
#define _SERVO_H_

//Inc
#include "stm32f10x.h"

//Var

//Fun
void Servo_Config(void);
#define Servo_Ch(tim,ch) (tim->CCR##ch)

#endif
