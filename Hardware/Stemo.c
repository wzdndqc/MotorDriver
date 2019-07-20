#include "Stemo.h"


//当前位置
int16_t Stemo_nowStep;
//期望位置
int16_t Stemo_setStep;

void Stemo_DIR_int()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(Stemo_GPIO_CLK, ENABLE);  //使能GPIO外设时钟使能
	
	GPIO_InitStructure.GPIO_Pin = Stemo_DIR_GPIO_PIN; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Stemo_DIR_GPIO_PORT, &GPIO_InitStructure);//初始化GPIO
	
	GPIO_InitStructure.GPIO_Pin = Stemo_STEP_GPIO_PIN ; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Stemo_STEP_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIO
}

void Stemo_StepPri(int16_t SP)
{
	//当前位置
    Stemo_nowStep = 0;
	//期望位置
    Stemo_setStep = SP;
}

void STEMO_DRI() //步进电机循环服务函数，用户需要循环调用
{
	if (Stemo_nowStep<Stemo_setStep)
	{
		GPIOC->BSRR = Stemo_DIR_GPIO_PIN;
		if(Stemo_nowStep<Stemo_setStep)
		{
			GPIOC->BSRR = Stemo_STEP_GPIO_PIN;
			GPIOC->BRR = Stemo_STEP_GPIO_PIN;
			Stemo_nowStep++;
		}
	}
	else if(Stemo_nowStep>Stemo_setStep)
	{
		GPIOC->BRR = Stemo_DIR_GPIO_PIN;
		if(Stemo_nowStep>Stemo_setStep)
		{
			GPIOC->BSRR = Stemo_STEP_GPIO_PIN;
			GPIOC->BRR = Stemo_STEP_GPIO_PIN;
			Stemo_nowStep--;
		}
	}
	
}	


