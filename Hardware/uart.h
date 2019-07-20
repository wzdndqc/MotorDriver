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

//缓冲区大小
#define UART_BUFFER_SIZE 64
//printf所用的USART
#define UART_PRINTF USART2
//缓冲区发送所用的USART
#define UART_BUFFER USART2
//缓冲区
extern uint8_t Uart_TxBufs[UART_BUFFER_SIZE];
extern uint8_t Uart_RxBufs[UART_BUFFER_SIZE];
//当前缓冲区队头索引
extern uint16_t Uart_iHead;
//当前缓冲区队尾索引
extern uint16_t Uart_iTail;
//接收缓冲区指针
extern uint8_t *Uart_pRx;

//Fun
/*
 * @brief	Init Usart
 * @param	usart	USART2,USART3
 * @param	baud	Baudrate波特率
 * @retval	None
 */
void Uart_Config(USART_TypeDef *usart, uint32_t baud);
/*
 * @brief	将数据存入发送缓冲区并发送字符串，输入的字符串数组必须有\0结尾。
 * @param	str		字符串，\0作为结束符。
 * @retval	数据是否成功存入缓冲区:成功返回SUCCESS并开始串口发送，
 * 	若超出缓冲区大小则返回ERROR，需减小str长度或稍后重试。
 */
ErrorStatus Uart_SendString(char *str);
/*
 * @brief	缓冲区收发处理函数，使用缓冲区时用户需要在串口中断服务函数中调用此函数。
 * @param	None
 * @retval	None
 */
void Uart_BufsHandler(void);

/*
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
