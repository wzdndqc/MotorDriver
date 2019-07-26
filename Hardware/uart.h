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

//printf所用的USART
#define UART_PRINTF USART2
//缓冲区结构体
typedef struct
{
	//缓冲区大小
	USART_TypeDef *USARTx;
	//缓冲区发送所用的USART
	uint16_t size;
	//缓冲区
	uint8_t *buffer;
	//当前缓冲区队头索引
	uint16_t iHead;
	//当前缓冲区队尾索引
	uint16_t iTail;
} UART_Buffer;

//Fun
/*
 * @brief	初始化发送缓冲区结构体
 * @param	buffer	缓冲区的结构体指针
 * @param	USARTx	用于发送缓冲区内容的串口如USART2
 * @param	bufs	缓冲区数组
 * @param	size	缓冲区数组长度
 * @retval	None
 */
void Uart_InitBuffer(UART_Buffer *buffer, USART_TypeDef *USARTx, uint8_t *bufs, uint16_t size);
/*
 * @brief	Init Usart
 * @param	usart	USART2,USART3
 * @param	baud	Baudrate波特率
 * @retval	None
 */
void Uart_Config(USART_TypeDef *usart, uint32_t baud);
/*
 * @brief	将数据存入发送缓冲区并发送字符串，输入的字符串数组必须有\0结尾。
 * @param	buffer	代表串口的缓冲区结构体
 * @param	str		字符串，\0作为结束符。
 * @retval	数据是否成功存入缓冲区:成功返回SUCCESS并开始串口发送，
 * 	若超出缓冲区大小则返回ERROR，需减小str长度或稍后重试。
 */
ErrorStatus Uart_SendString(UART_Buffer *buffer, char *str);
/*
 * @brief	缓冲区收发处理函数，使用缓冲区时用户需要在串口中断服务函数中调用此函数。
 * @param	buffer	代表串口的缓冲区结构体
 * @retval	None
 */
void Uart_BufsHandler(UART_Buffer *buffer);

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
