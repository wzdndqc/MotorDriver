/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "Stemo.h"
#include "uart.h"
#include "i2c.h"
#include "Driver/JY61.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern uint32_t Clock_LED;
extern uint32_t Clock_Mtr;
extern UART_Buffer buffer2;
extern I2C_BufferTypeDef I2C_buffer2;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  Stemo_Loop();
  if (Clock_LED)
    Clock_LED--;
  if (Clock_Mtr)
    Clock_Mtr--;
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles USART2 global interrupt request.
  * @param  None
  * @retval None
  */
void USART2_IRQHandler(void)
{
  Uart_BufsHandler(&buffer2);
}

/**
  * @brief  This function handles USART3 global interrupt request.
  * @param  None
  * @retval None
  */
void USART3_IRQHandler(void)
{
  if (USART3->SR & USART_SR_RXNE)
  {
    JY61_RxData(USART3->DR);
  }
}

#define IS_EVENT(sr, evt) ((sr & evt) == evt)

/**
  * @brief  This function handles I2C2 Event interrupt request.
  * @param  None
  * @retval None
  */
void I2C2_EV_IRQHandler(void)
{
  //DBG
  GPIOB->BSRR = GPIO_BSRR_BS9;
  I2C_BufsHandler(&I2C_buffer2);
  //DBG
  GPIOB->BSRR = GPIO_BSRR_BR9;
}
/* {
  uint32_t SR, SR2;

  //DBG
  GPIOB->BSRR = GPIO_BSRR_BS9;

  SR = I2C2->SR1;
  SR2 = I2C2->SR2;
  SR2 = SR2 << 16;
  SR = (SR | SR2);

  if (IS_EVENT(SR, I2C_EVENT_MASTER_MODE_SELECT))
  {
    I2C2->DR = (I2C2->CR2 & I2C_CR2_ITBUFEN) ? 0xD0 : 0xD1;
  }
  else if (IS_EVENT(SR, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    I2C2->DR = 0x75;
    //Close ITBUFEN
    I2C2->CR2 &= ~I2C_CR2_ITBUFEN;
  }
  else if (IS_EVENT(SR, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    //Read Mode
    I2C2->CR1 |= I2C_CR1_START; //Restart
    I2C2->DR = 0x00;
  }
  else if (IS_EVENT(SR, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
  {
    I2C2->CR2 |= I2C_CR2_ITBUFEN;
    //If only one to receive
    I2C2->CR1 &= ~I2C_CR1_ACK;
    I2C2->CR1 |= I2C_CR1_START;
    //I2C2->CR1 |= I2C_CR1_STOP;
  }
  else if (IS_EVENT(SR, I2C_EVENT_MASTER_BYTE_RECEIVED))
  {
    if (I2C2->DR == 0x68)
      Uart_SendString(&buffer2, "Its!");
    else
      Uart_SendString(&buffer2, "Oop!");
  }

  //DBG
  GPIOB->BSRR = GPIO_BSRR_BR9;
} */

/**
  * @brief  This function handles I2C2 Error interrupt request.
  * @param  None
  * @retval None
  */
void I2C2_ER_IRQHandler(void)
{
  if (I2C2->SR1 & I2C_SR1_BERR)
  {
    I2C2->SR1 &= ~I2C_SR1_BERR;
    Uart_SendString(&buffer2, "BERR!");
  }
  if (I2C2->SR1 & I2C_SR1_ARLO)
  {
    I2C2->SR1 &= ~I2C_SR1_ARLO;
    Uart_SendString(&buffer2, "ARLO!");
  }
  if (I2C2->SR1 & I2C_SR1_AF)
  {
    I2C2->SR1 &= ~I2C_SR1_AF;
    Uart_SendString(&buffer2, "AF!");
  }
  if (I2C2->SR1 & I2C_SR1_OVR)
  {
    I2C2->SR1 &= ~I2C_SR1_OVR;
    Uart_SendString(&buffer2, "OVR!");
  }
  if (I2C2->SR1 & I2C_SR1_PECERR)
  {
    I2C2->SR1 &= ~I2C_SR1_PECERR;
    Uart_SendString(&buffer2, "PECERR!");
  }
  if (I2C2->SR1 & I2C_SR1_TIMEOUT)
  {
    I2C2->SR1 &= ~I2C_SR1_TIMEOUT;
    Uart_SendString(&buffer2, "TIMEOUT!");
  }
  if (I2C2->SR1 & I2C_SR1_SMBALERT)
  {
    I2C2->SR1 &= ~I2C_SR1_SMBALERT;
    Uart_SendString(&buffer2, "SMBALERT!");
  }
  if (I2C2->SR2 & I2C_SR2_MSL)
  {
    I2C2->CR1 |= I2C_CR1_STOP;
  }
}

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
