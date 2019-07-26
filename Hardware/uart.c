/*
Uart Driver And BlueTooth Electronics

Hardware res:

USART2_TX : PA2
USART2_RX : PA3
USART3_TX : PC10
USART3_RX : PC11

Software res:

USART2
USART3

GPIOA GPIOC
*/

//Inc
#include "uart.h"

//Var

//Fun
void Uart_InitBuffer(UART_Buffer *buffer, USART_TypeDef *USARTx, uint8_t *bufs, uint16_t size)
{
	buffer->USARTx = USARTx;
	buffer->buffer = bufs;
	buffer->size = size;
	buffer->iHead = 0;
	buffer->iTail = size - 1;
}
void Uart_Config(USART_TypeDef *usart, uint32_t baud)
{
	GPIO_InitTypeDef GPIO_InitStructureRx;
	GPIO_InitTypeDef GPIO_InitStructureTx;
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	//RCC | GPIO | NVIC
	//AFIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	//RxGPIO
	GPIO_InitStructureTx.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructureTx.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructureRx.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	//NVIC
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	if (usart == USART1)
	{
		RCC_APB2PeriphClockCmd(
			RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
		GPIO_InitStructureTx.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructureRx.GPIO_Pin = GPIO_Pin_10;
		GPIO_Init(GPIOA, &GPIO_InitStructureTx);
		GPIO_Init(GPIOA, &GPIO_InitStructureRx);
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	}
	else if (usart == USART2)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		GPIO_InitStructureTx.GPIO_Pin = GPIO_Pin_2;
		GPIO_InitStructureRx.GPIO_Pin = GPIO_Pin_3;
		GPIO_Init(GPIOA, &GPIO_InitStructureTx);
		GPIO_Init(GPIOA, &GPIO_InitStructureRx);
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	}
	else if (usart == USART3)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
		GPIO_InitStructureTx.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructureRx.GPIO_Pin = GPIO_Pin_11;
		GPIO_Init(GPIOC, &GPIO_InitStructureTx);
		GPIO_Init(GPIOC, &GPIO_InitStructureRx);
		GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, ENABLE);
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	}
	NVIC_Init(&NVIC_InitStructure);

	//USART
	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(usart, &USART_InitStructure);
	USART_ITConfig(usart, USART_IT_RXNE, ENABLE);
	USART_Cmd(usart, ENABLE);
	usart->SR;
}

#define buffer_add(buffer, val) ((val >= buffer->size - 1) ? 0 : val + 1)
ErrorStatus Uart_SendString(UART_Buffer *buffer, char *str)
{
	uint16_t iHead = buffer->iHead;
	while (*str)
	{
		if (iHead != buffer->iTail)
		{
			buffer->buffer[iHead] = *(str++);
			iHead = buffer_add(buffer, iHead);
		}
		else
		{
			return ERROR;
		}
	}
	buffer->iHead = iHead;
	USART_ITConfig(buffer->USARTx, USART_IT_TXE, ENABLE);
	return SUCCESS;
}

void Uart_BufsHandler(UART_Buffer *buffer)
{
	if (buffer->USARTx->SR & USART_SR_TXE)
	{
		//判断缓冲区是否还有数据
		if (buffer_add(buffer, buffer->iTail) != buffer->iHead)
		{
			buffer->iTail = buffer_add(buffer, buffer->iTail);
			buffer->USARTx->DR = buffer->buffer[buffer->iTail];
		}
		else
		{
			USART_ITConfig(buffer->USARTx, USART_IT_TXE, DISABLE);
		}
	}
	if (buffer->USARTx->SR & USART_SR_RXNE)
	{
		//接收中断保护
		buffer->USARTx->DR;
	}
}

PUTCHAR_PROTOTYPE
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	USART_SendData(USART2, (uint8_t)ch);

	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
	{
	}

	return ch;
}
