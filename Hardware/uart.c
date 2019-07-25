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
uint16_t Uart_iHead = 0;
uint16_t Uart_iTail = UART_BUFFER_SIZE - 1;
uint8_t Uart_TxBufs[UART_BUFFER_SIZE];
uint8_t Uart_RxBufs[UART_BUFFER_SIZE];
uint8_t *Uart_pRx = Uart_RxBufs;

//Fun
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

	//Init
	Uart_RxBufs[0] = 0;
}

#define buffer_add(val) ((val >= UART_BUFFER_SIZE - 1) ? 0 : val + 1)
ErrorStatus Uart_SendString(char *str)
{
	uint16_t iHead = Uart_iHead;
	while (*str)
	{
		if (iHead != Uart_iTail)
		{
			Uart_TxBufs[iHead] = *(str++);
			iHead = buffer_add(iHead);
		}
		else
		{
			return ERROR;
		}
	}
	Uart_iHead = iHead;
	USART_ITConfig(UART_BUFFER, USART_IT_TXE, ENABLE);
	return SUCCESS;
}

void Uart_BufsHandler(void)
{
	if (UART_BUFFER->SR & USART_SR_TXE)
	{
		//判断缓冲区是否还有数据
		if (buffer_add(Uart_iTail) != Uart_iHead)
		{
			Uart_iTail = buffer_add(Uart_iTail);
			UART_BUFFER->DR = Uart_TxBufs[Uart_iTail];
		}
		else
		{
			USART_ITConfig(UART_BUFFER, USART_IT_TXE, DISABLE);
		}
	}
	if (UART_BUFFER->SR & USART_SR_RXNE)
	{
		//接收
		if (Uart_pRx - Uart_RxBufs < UART_BUFFER_SIZE)
		{
			*Uart_pRx = UART_BUFFER->DR;
			*(Uart_pRx++) = 0;
		}
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
