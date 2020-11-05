/**
 * 
 * @file	i2c.h
 * @author	Tamap
 * @date	2020-10-17
 * @brief	I2C Driver
 * 
 * IO res:
 * 
 * I2C2_SCL : PB10
 * I2C2_SDA : PB11
 * 
 * Hardware res:
 * 
 * I2C2
 * 
 * GPIOB
*/

#include "i2c.h"

void I2C_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	I2C_InitTypeDef I2C_InitStructure;

	//RCC | GPIO | NVIC
	//GPIO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//NVIC
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//GPIO-I2C2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//NVIC-I2C2
	NVIC_InitStructure.NVIC_IRQChannel = I2C2_EV_IRQn;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = I2C2_ER_IRQn;
	NVIC_Init(&NVIC_InitStructure);
	//I2C2
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = I2C2_SLAVE_ADDRESS7;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = I2C2_SPEED;
	//Init
	I2C_Init(I2C2, &I2C_InitStructure);
	I2C_Cmd(I2C2, ENABLE);
}

uint8_t I2C_TestWhileSTD(void)
{
	uint32_t timeout;
	uint8_t buf;

	timeout = 40960;
	while (I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY))
	{
		if ((timeout--) == 0)
			return 0;
	}

	I2C_GenerateSTART(I2C2, ENABLE);

	timeout = 0x1000;
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))
	{
		if ((timeout--) == 0)
			return 0;
	}

	I2C_Send7bitAddress(I2C2, 0xD0, I2C_Direction_Transmitter);

	timeout = 0x1000;
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if ((timeout--) == 0)
			return 0;
	}

	I2C_SendData(I2C2, 0x75);

	timeout = 0x1000;
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if ((timeout--) == 0)
			return 0;
	}

	I2C_GenerateSTART(I2C2, ENABLE);

	timeout = 0x1000;
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))
	{
		if ((timeout--) == 0)
			return 0;
	}

	I2C_Send7bitAddress(I2C2, 0xD0, I2C_Direction_Receiver);

	timeout = 0x1000;
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
	{
		if ((timeout--) == 0)
			return 0;
	}

	I2C_AcknowledgeConfig(I2C2, DISABLE);
	I2C_GenerateSTOP(I2C2, ENABLE);

	timeout = 0x1000;
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED))
	{
		if ((timeout--) == 0)
			return 0;
	}

	buf = I2C_ReceiveData(I2C2);

	I2C_AcknowledgeConfig(I2C2, ENABLE);

	timeout = 0x1000;
	while (I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY))
	{
		if ((timeout--) == 0)
			return 0;
	}

	return buf;
}

uint8_t I2C_TestWhileREG(void)
{
	uint32_t timeout;
	uint8_t buf;

	timeout = 4096;
	while (I2C2->SR2 & I2C_SR2_BUSY)
	{
		if ((timeout--) == 0)
			return 0;
	}

	I2C2->CR1 |= I2C_CR1_START;

	timeout = 4096;
	while (!(I2C2->SR1 & I2C_SR1_SB))
	{
		if ((timeout--) == 0)
			return 0;
	}

	I2C2->DR = 0xD0 | 0x00;

	timeout = 4096;
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if ((timeout--) == 0)
			return 0;
	}

	I2C_SendData(I2C2, 0x75);

	timeout = 4096;
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if ((timeout--) == 0)
			return 0;
	}

	I2C_GenerateSTART(I2C2, ENABLE);

	timeout = 4096;
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))
	{
		if ((timeout--) == 0)
			return 0;
	}

	I2C_Send7bitAddress(I2C2, 0xD0, I2C_Direction_Receiver);

	timeout = 4096;
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
	{
		if ((timeout--) == 0)
			return 0;
	}

	I2C_AcknowledgeConfig(I2C2, DISABLE);
	I2C_GenerateSTOP(I2C2, ENABLE);

	timeout = 4096;
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED))
	{
		if ((timeout--) == 0)
			return 0;
	}

	buf = I2C_ReceiveData(I2C2);

	I2C_AcknowledgeConfig(I2C2, ENABLE);

	return buf;
}
