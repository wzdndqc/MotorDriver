/*
Uart Driver And BlueTooth Electronics

IO res:

USART2_TX : PA2
USART2_RX : PA3
USART3_TX : PC10
USART3_RX : PC11

Hardware res:

USART2
USART3

GPIOA GPIOC
*/
#ifndef _UART_H_
#define _UART_H_

//Inc
#include "stm32f10x.h"
#include "stdio.h"

//Var

//Fun
void Uart_Config(USART_TypeDef *usart, uint32_t baud, FunctionalState isIT);

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

#endif
