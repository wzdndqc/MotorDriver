#ifndef _ADC_H
#define	_ADC_H


#include "stm32f10x.h"

// ADC GPIO宏定义
// 注意：用作ADC采集的IO必须没有复用，否则采集电压会有影响
#define    ADC_GPIO_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define    ADC_GPIO_CLK                  RCC_APB2Periph_GPIOB  
#define    ADC_PORT                      GPIOB

// 转换通道个数
#define    NOFCHANEL						2			

#define    ADC_PIN1                      GPIO_Pin_0
#define    ADC_CHANNEL1                  ADC_Channel_8

#define    ADC_PIN2                      GPIO_Pin_1
#define    ADC_CHANNEL2                  ADC_Channel_9


// ADC 编号选择
// 可以是 ADC1/2，如果使用ADC3，中断相关的要改成ADC3的
#define    ADC_APBxClock_FUN             RCC_APB2PeriphClockCmd
#define    ADC_x                         ADC1
#define    ADC_CLK                       RCC_APB2Periph_ADC1

// ADC 通道宏定义

#define    ADC_DMA_CLK                   RCC_AHBPeriph_DMA1
#define    ADC_DMA_CHANNEL               DMA1_Channel1

// ADC 中断相关宏定义
#define    ADC_IRQ                       ADC1_2_IRQn
#define    ADC_IRQHandler                ADC1_2_IRQHandler

//
void ADCx_Init(void);
//直接读取adc1或adc2的值
#define ADC1_num      Double_ADC_ConvertedValue[0]
#define ADC2_num      Double_ADC_ConvertedValue[1]

#endif /* __BSP_ADC_H */


