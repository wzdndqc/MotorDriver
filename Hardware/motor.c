/*
 * @file	motor.c
 * @auther	Tamap, HHJ
 * @brief	DC Motor Driver
 * @date	2019-7-16
*/
//Def
#define RCC_GPIO(gpio) RCC_APB2Periph_##gpio

//Inc
#include "motor.h"

//Var

//Fun
void Motor_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	//RCC
	RCC_APB2PeriphClockCmd(
		RCC_GPIO(GPIOA) | RCC_GPIO(GPIOB) | RCC_GPIO(GPIOC) | RCC_GPIO(GPIOD) |
			RCC_APB2Periph_AFIO,
		ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	//GPIO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//M1
	GPIO_InitStructure.GPIO_Pin = PIN_M1_N1;
	GPIO_Init(GPIO_M1_N1, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = PIN_M1_N2;
	GPIO_Init(GPIO_M1_N2, &GPIO_InitStructure);
	//M2
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	GPIO_InitStructure.GPIO_Pin = PIN_M2_N1;
	GPIO_Init(GPIO_M2_N1, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = PIN_M2_N2;
	GPIO_Init(GPIO_M2_N2, &GPIO_InitStructure);
	//M3
	GPIO_InitStructure.GPIO_Pin = PIN_M3_N1;
	GPIO_Init(GPIO_M3_N1, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = PIN_M3_N2;
	GPIO_Init(GPIO_M3_N2, &GPIO_InitStructure);
	//M4
	GPIO_InitStructure.GPIO_Pin = PIN_M4_N1;
	GPIO_Init(GPIO_M4_N1, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = PIN_M4_N2;
	GPIO_Init(GPIO_M4_N2, &GPIO_InitStructure);
	//PWM
	GPIO_InitStructure.GPIO_Pin = PIN_M1_PWM | PIN_M2_PWM | PIN_M3_PWM | PIN_M4_PWM;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//TIM Base : 1us Psc 20ms Per
	TIM_TimeBaseStructure.TIM_Period = 20000 - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock / 1000000) - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	//TIM OC
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_Pulse = 1500;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	TIM_Cmd(TIM1, ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}
void Motor_Output(uint8_t index, int16_t pwm)
{
	uint16_t ccr;
	if (pwm < 0)
	{
		ccr = 0 - pwm;
		switch (index)
		{
		case 1:
			GPIO_M1_N1->BRR = PIN_M1_N1;
			GPIO_M1_N2->BSRR = PIN_M1_N2;
			TIM1->CCR1 = ccr;
			break;
		case 2:
			GPIO_M2_N1->BRR = PIN_M2_N1;
			GPIO_M2_N2->BSRR = PIN_M2_N2;
			TIM1->CCR2 = ccr;
			break;
		case 3:
			GPIO_M3_N1->BRR = PIN_M3_N1;
			GPIO_M3_N2->BSRR = PIN_M3_N2;
			TIM1->CCR3 = ccr;
			break;
		case 4:
			GPIO_M4_N1->BRR = PIN_M4_N1;
			GPIO_M4_N2->BSRR = PIN_M4_N2;
			TIM1->CCR4 = ccr;
			break;

		default:
			break;
		}
	}
	else
	{
		ccr = pwm;
		switch (index)
		{
		case 1:
			GPIO_M1_N1->BSRR = PIN_M1_N1;
			GPIO_M1_N2->BRR = PIN_M1_N2;
			TIM1->CCR1 = ccr;
			break;
		case 2:
			GPIO_M2_N1->BSRR = PIN_M2_N1;
			GPIO_M2_N2->BRR = PIN_M2_N2;
			TIM1->CCR2 = ccr;
			break;
		case 3:
			GPIO_M3_N1->BSRR = PIN_M3_N1;
			GPIO_M3_N2->BRR = PIN_M3_N2;
			TIM1->CCR3 = ccr;
			break;
		case 4:
			GPIO_M4_N1->BSRR = PIN_M4_N1;
			GPIO_M4_N2->BRR = PIN_M4_N2;
			TIM1->CCR4 = ccr;
			break;

		default:
			break;
		}
	}
}
void Motor_Stop(uint8_t index)
{
	switch (index)
	{
	case 1:
		GPIO_M1_N1->BRR = PIN_M1_N1;
		GPIO_M1_N2->BRR = PIN_M1_N2;
		break;
	case 2:
		GPIO_M2_N1->BRR = PIN_M2_N1;
		GPIO_M2_N2->BRR = PIN_M2_N2;
		break;
	case 3:
		GPIO_M3_N1->BRR = PIN_M3_N1;
		GPIO_M3_N2->BRR = PIN_M3_N2;
		break;
	case 4:
		GPIO_M4_N1->BRR = PIN_M4_N1;
		GPIO_M4_N2->BRR = PIN_M4_N2;
		break;

	default:
		break;
	}
}
uint16_t Motor_GetStep(uint8_t index);
