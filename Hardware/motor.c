/*
DC Motor Driver
*/
//Def
#define RCC_GPIO(gpio) RCC_APB2Periph_##gpio

//Inc
#include "motor.h"

//Var

//Fun
void Motor_Config(void)
{
	//RCC
	RCC_APB2PeriphClockCmd(
		RCC_GPIO(GPIOA) | RCC_GPIO(GPIOB) | RCC_GPIO(GPIOC) | RCC_GPIO(GPIOD),
		ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
}
void Motor_Output(uint8_t index, int16_t pwm);
