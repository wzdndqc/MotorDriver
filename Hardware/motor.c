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
	TIM_TimeBaseStructure.TIM_Period = MOTOR_PERIOD - 1;
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
void Motor_Encoder_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4 | RCC_APB1Periph_TIM5, ENABLE); //使能定时器4的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);										   //使能端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; //端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //浮空输入

	//M1
	GPIO_InitStructure.GPIO_Pin = PIN_M1_EA;
	GPIO_Init(GPIO_M1_N1, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = PIN_M1_EB;
	GPIO_Init(GPIO_M1_N2, &GPIO_InitStructure);
	//M2
	GPIO_InitStructure.GPIO_Pin = PIN_M2_EA;
	GPIO_Init(GPIO_M2_N1, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = PIN_M2_EB;
	GPIO_Init(GPIO_M2_N2, &GPIO_InitStructure);
	//M3
	GPIO_InitStructure.GPIO_Pin = PIN_M3_EA;
	GPIO_Init(GPIO_M3_N1, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = PIN_M3_EB;
	GPIO_Init(GPIO_M3_N2, &GPIO_InitStructure);
	//M4
	GPIO_InitStructure.GPIO_Pin = PIN_M4_EA;
	GPIO_Init(GPIO_M4_N1, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = PIN_M4_EB;
	GPIO_Init(GPIO_M4_N2, &GPIO_InitStructure);

	//编码器
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;					// 预分频器
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF;					//设定计数器自动重装值
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//选择时钟分频：不分频
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising); //使用编码器模式3
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising); //使用编码器模式3
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising); //使用编码器模式3
	TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising); //使用编码器模式3
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	TIM_ICInit(TIM5, &TIM_ICInitStructure);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update); //清除TIM的更新标志位
	TIM_ClearFlag(TIM3, TIM_FLAG_Update); //清除TIM的更新标志位
	TIM_ClearFlag(TIM4, TIM_FLAG_Update); //清除TIM的更新标志位
	TIM_ClearFlag(TIM5, TIM_FLAG_Update); //清除TIM的更新标志位
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
	//Reset counter
	TIM_SetCounter(TIM2, 0);
	TIM_SetCounter(TIM3, 0);
	TIM_SetCounter(TIM4, 0);
	TIM_SetCounter(TIM5, 0);
	TIM_Cmd(TIM2, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
	TIM_Cmd(TIM4, ENABLE);
	TIM_Cmd(TIM5, ENABLE);
}
uint16_t Motor_GetStep(uint8_t index)
{
	uint16_t Encoder_TIM;
	switch (index)
	{
	case 1:
		Encoder_TIM = TIM2->CNT;
		;
		break;
	case 2:
		Encoder_TIM = TIM3->CNT;
		;
		break;
	case 3:
		Encoder_TIM = TIM4->CNT;
		;
		break;
	case 4:
		Encoder_TIM = TIM5->CNT;
		;
		break;
	default:
		Encoder_TIM = 0;
	}
	return Encoder_TIM;
}
