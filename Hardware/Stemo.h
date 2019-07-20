#ifndef  STEMO_H
#define  STEMO_H

#include "stm32f10x.h"

#define Stemo_TIM3_CLK	(RCC_APB1Periph_TIM3)
#define Stemo_GPIO_CLK	(RCC_APB2Periph_GPIOC)

#define Stemo_DIR_GPIO_PIN  GPIO_Pin_4
#define Stemo_DIR_GPIO_PORT	GPIOC
#define Stemo_STEP_GPIO_PIN  GPIO_Pin_5
#define Stemo_STEP_GPIO_PORT	GPIOC

void Stemo_StepPri(int16_t SP);
void Stemo_DIR_int(void);
void STEMO_DRI (void);//步进电机循环服务函数，用户需要循环调用
#endif 
