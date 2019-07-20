/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "servo.h"
#include "uart.h"
#include "motor.h"
#include "Stemo.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t Clock_LED = 0;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void GPIO_Config(void);

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	uint8_t n = 0;
	SysTick_Config(SystemCoreClock / 1000);

	GPIO_Config();



/*  步进电机---------------------------------------------------------*/
	Stemo_DIR_int();//步进电机IO初�?�化
	Stemo_StepPri(10);//该参数为步进电机期望步数，用户自行修�?
  	STEMO_DRI(); //步进电机�?�?服务函数，用户需要循�?调用

	  
/*  ---------------------------------------------------------*/
	//Hardware

	Servo_Config();
	Servo_Ch(TIM8, 1) = 1400;
	Uart_Config(USART2, 9600, DISABLE);
	printf("\r\n");

	//Test
	Motor_Config();
	Motor_Encoder_Config();
	Motor_Output(2, 0);
	Motor_Stop(2);
	//Clock_LED = 3000;

	/* Infinite loop */
	while (1)
	{
		if (Clock_LED == 0)
		{
			Clock_LED = 500;
			GPIOC->ODR ^= GPIO_ODR_ODR13;
			Servo_Ch(TIM8, 1) += (1500 - Servo_Ch(TIM8, 1)) * 2;
			printf("%d\n",Motor_GetStep(2));
			//printf("ADC:%3d\r\n", n);
			//Test
			// Motor_Output(2, ((int16_t)((n > 30)?60-n:n) - 15)*500);
			//Motor_Output(2, 0);
			n++;
			if (n > 60)
				n = 0;
		}
	}
}

void GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	//LED
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	//Key
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1)
	{
	}
}
#endif

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
