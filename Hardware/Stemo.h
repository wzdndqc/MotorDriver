#ifndef STEMO_H
#define STEMO_H

#include "stm32f10x.h"

#define Stemo_TIM3_CLK (RCC_APB1Periph_TIM3)
#define Stemo_GPIO_CLK (RCC_APB2Periph_GPIOC)

#define Stemo_DIR_GPIO_PIN GPIO_Pin_4
#define Stemo_DIR_GPIO_PORT GPIOC
#define Stemo_STEP_GPIO_PIN GPIO_Pin_5
#define Stemo_STEP_GPIO_PORT GPIOC

//当前位置
extern int16_t Stemo_nowStep;
//期望位置
extern int16_t Stemo_setStep;

/** 
  * @brief	设置步进
  */
#define Stemo_AddStep(sp) Stemo_setStep += (int16_t)(sp)
#define Stemo_SetStep(sp) Stemo_setStep = (sp)
#define Stemo_SetPos(p) Stemo_nowStep = (p)
/** 
  * @brief	步进电机初始化函数
  */
void Stemo_Config(void);
/** 
  * @brief	步进电机循环服务函数，用户需要定时调用
  */
void Stemo_Loop(void);
#endif
