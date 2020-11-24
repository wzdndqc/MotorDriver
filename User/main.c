/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "servo.h"
#include "uart.h"
#include "motor.h"
#include "Stemo.h"
#include "adc.h"
#include "Driver/JY61.h"
#include "i2c.h"
#include "Driver/MPU6050.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t Clock_LED = 0;
uint32_t Clock_Mtr = 0;
//UART
UART_Buffer buffer2;
uint8_t bufStr2[256];
char str[48];
//I2C
I2C_BufferTypeDef I2C_buffer2;
I2C_IdxBuffer I2C_idxs2[16];
uint8_t I2C_data2[256];
ErrorStatus staTest;
uint8_t dataTest;
MPU_Datas angleDataR;
MPU_Datas angleData;

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
	uint16_t n = 0;
	uint16_t last_step = 0;
	int16_t add_step = 0;
	SysTick_Config(SystemCoreClock / 1000);

	Clock_LED = 200;
	while(Clock_LED);

	GPIO_Config();

	//Hardware
	//Uart
	Uart_Config(USART2, 115200);
	Uart_InitBuffer(&buffer2, USART2, bufStr2, sizeof(bufStr2));
	Uart_Config(USART3, 115200);
	//ADC
	ADC_Config();
	//Stemo
	Stemo_Config();
	//Servo
	Servo_ConfigT8(20000);
	//Motor
	Motor_Encoder_Config();
	Motor_Config(2000);
	Motor_Stop(4);
	//I2C
	I2C_Config();

	//TestBuffer
	I2C_InitBuffer(&I2C_buffer2, I2C2, I2C_idxs2, sizeof(I2C_idxs2)/sizeof(I2C_IdxBuffer), I2C_data2, sizeof(I2C_data2));
	staTest = I2C_ReadMem(&I2C_buffer2, 0xD1, 0x75, &dataTest, 1 - 1);
	staTest = I2C_WriteBuffer(&I2C_buffer2, 0xD0, REG_PWR_MGMT_1, (uint8_t[]){0x00}, 1 - 1);
	staTest = I2C_WriteBuffer(&I2C_buffer2, 0xD0, REG_SMPLRT_DIV, (uint8_t[]){0x07}, 1 - 1);
	staTest = I2C_WriteBuffer(&I2C_buffer2, 0xD0, REG_CONFIG, (uint8_t[]){0x06}, 1 - 1);
	staTest = I2C_WriteBuffer(&I2C_buffer2, 0xD0, REG_ACCEL_CONFIG, (uint8_t[]){0x00}, 1 - 1);
	staTest = I2C_WriteBuffer(&I2C_buffer2, 0xD0, REG_GYRO_CONFIG, (uint8_t[]){0x00}, 1 - 1);
	// staTest = I2C_ReadMem(&I2C_buffer2, 0xD1, 0x75, &dataTest, 1 - 1);
	//I2C_TestWhileREG();

	//Start I2C
	//I2C2->CR2 |= (I2C_CR2_ITBUFEN | I2C_CR2_ITEVTEN | I2C_CR2_ITERREN);
	//I2C2->CR1 |= I2C_CR1_START;

	/* Infinite loop */
	while (1)
	{
		if (Clock_Mtr == 0)
		{
			Clock_Mtr = 20;
			//Motor output
			Motor_Output(2, ((int16_t)2048 - ADC_Value1) * 10);
			//Servo output
			Servo_Ch(TIM8, 1) = 500 + ADC_Value1 / 4;
			Servo_Ch(TIM8, 2) = 500 + ADC_Value2 / 4;
			Servo_Ch(TIM8, 3) += (1500 - Servo_Ch(TIM8, 3)) * 2;
			//Step motor output
			add_step = (int16_t)(Motor_GetStep(2) - last_step);
			last_step = Motor_GetStep(2);
			Stemo_SetStep(add_step);
			//Uart print to USART2
			sprintf(str, "ADC1:%05d\r\n", ADC_Value1);
			Uart_SendString(&buffer2, str);
			//Get MPU
			((uint8_t *)&angleData)[0] = ((uint8_t *)&angleDataR)[1];
			((uint8_t *)&angleData)[1] = ((uint8_t *)&angleDataR)[0];
			((uint8_t *)&angleData)[2] = ((uint8_t *)&angleDataR)[3];
			((uint8_t *)&angleData)[3] = ((uint8_t *)&angleDataR)[2];
			((uint8_t *)&angleData)[4] = ((uint8_t *)&angleDataR)[5];
			((uint8_t *)&angleData)[5] = ((uint8_t *)&angleDataR)[4];
			I2C_ReadMem(&I2C_buffer2, 0xD1, REG_ACCEL_XOUT_H, (uint8_t *)&angleDataR, 6 - 1);
			I2C_ReadMem(&I2C_buffer2, 0xD1, REG_GYRO_XOUT_H, (uint8_t *)(&angleDataR.Gx), 6 - 1);
		}
		if (Clock_LED == 0)
		{
			Clock_LED = 200;
			GPIOC->ODR ^= GPIO_ODR_ODR13;
			sprintf(str, "ADC1:%05d\r\n", ADC_Value1);
			Uart_SendString(&buffer2, str);
			sprintf(str, "Step:%05d\r\n", Motor_GetStep(2));
			Uart_SendString(&buffer2, str);
			sprintf(str, "BUG:%05d\r\n", (n > 30) ? 60 - n : n);
			Uart_SendString(&buffer2, str);
			sprintf(str, "Ang:%.3f  %.3f  %.3f    \r\n", (float)JY61_stcAngle.Angle[0] / 32768 * 180, (float)JY61_stcAngle.Angle[1] / 32768 * 180, (float)JY61_stcAngle.Angle[2] / 32768 * 180);
			Uart_SendString(&buffer2, str);
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

	//DBG
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

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
