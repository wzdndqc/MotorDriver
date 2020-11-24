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

void I2C_InitBuffer(I2C_BufferTypeDef *buffer, I2C_TypeDef *I2Cx, I2C_IdxBuffer *idxBufs, uint16_t idxSize, uint8_t *dataBufs, uint16_t dataSize)
{
	buffer->I2Cx = I2Cx;

	buffer->IdxBuffer = idxBufs;
	buffer->IdxSize = idxSize;
	buffer->IdxEmpty = 1;
	buffer->pIHead = idxBufs;
	buffer->pITail = idxBufs;

	buffer->DataBuffer = dataBufs;
	buffer->DataSize = dataSize;
	buffer->DataEmpty = 1;
	buffer->pDHead = dataBufs;
	buffer->pDTail = dataBufs;

	buffer->ReTimes = 0;
}

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
//Enable transmit
void I2C_TransEnable(I2C_BufferTypeDef *buffer)
{
	I2C_TypeDef *I2Cx = buffer->I2Cx;

	//If is empty
	if (buffer->pIHead == buffer->pITail && buffer->IdxEmpty)
		return;
	//If is Busy Or Starting/Stoping
	if (I2Cx->SR2 & I2C_SR2_BUSY || I2Cx->CR1 & (I2C_CR1_START | I2C_CR1_STOP))
		return;

	//Start I2C
	I2C2->CR2 |= (I2C_CR2_ITBUFEN | I2C_CR2_ITEVTEN | I2C_CR2_ITERREN);
	I2C2->CR1 |= I2C_CR1_START;
}

//Write
ErrorStatus I2C_WriteBuffer(I2C_BufferTypeDef *buffer, uint8_t addr, uint8_t reg, uint8_t *datas, uint8_t len_1)
{
	uint8_t *pDHeadTmp;
	uint8_t *pDataEnd;
	I2C_IdxBuffer *pIHeadTmp;
	I2C_IdxBuffer *pIdxEnd;
	//IdxFull
	if (buffer->pIHead == buffer->pITail)
		if (!buffer->IdxEmpty)
			return ERROR;
	//DatFull
	if (buffer->pDHead == buffer->pDTail)
		if (!buffer->DataEmpty)
			return ERROR;
	//Empty Len(Size)	Len 1 ~ 256 Len_1 0 ~ 255
	if (buffer->pDHead < buffer->pDTail)
	{
		if (len_1 > buffer->pDTail - buffer->pDHead - 1)
			return ERROR;
	}
	else
	{
		if (len_1 > buffer->pDTail + buffer->DataSize - buffer->pDHead - 1)
			return ERROR;
	}

	//Save Head
	pDHeadTmp = buffer->pDHead;
	pIHeadTmp = buffer->pIHead;
	//Count End
	pDataEnd = buffer->DataBuffer + buffer->DataSize - 1;
	pIdxEnd = buffer->IdxBuffer + buffer->IdxSize - 1;

	//Set Datas head
	pIHeadTmp->Data = buffer->pDHead;
	pIHeadTmp->Len = len_1;
	//Write datas
	do
	{
		*(pDHeadTmp++) = *(datas++);
		if (pDHeadTmp > pDataEnd)
			pDHeadTmp = buffer->DataBuffer;
	} while (len_1--);
	//Warrning!len_1 has been change

	//Set idxSct
	pIHeadTmp->Addr = addr;
	pIHeadTmp->Reg = reg;

	//DISABLE IT
	buffer->I2Cx->CR2 &= ~(I2C_CR2_ITERREN | I2C_CR2_ITEVTEN);

	//Set Head
	buffer->pDHead = pDHeadTmp;
	if (buffer->pDHead == buffer->pDTail)
		buffer->DataEmpty = 0;
	if (pIHeadTmp < pIdxEnd)
		buffer->pIHead = pIHeadTmp + 1;
	else
		buffer->pIHead = buffer->IdxBuffer;
	if (buffer->pIHead == buffer->pITail)
		buffer->IdxEmpty = 0;

	//Enable EV ER IT
	I2C_TransEnable(buffer);

	//ENABLE IT
	buffer->I2Cx->CR2 |= (I2C_CR2_ITERREN | I2C_CR2_ITEVTEN);

	return SUCCESS;
}
//Read
ErrorStatus I2C_ReadMem(I2C_BufferTypeDef *buffer, uint8_t addr, uint8_t reg, uint8_t *datas, uint8_t len_1)
{
	I2C_IdxBuffer *pIHeadTmp;
	I2C_IdxBuffer *pIdxEnd;
	//IdxFull
	if (buffer->pIHead == buffer->pITail)
		if (!buffer->IdxEmpty)
			return ERROR;
	//Save Head
	pIHeadTmp = buffer->pIHead;
	//Count End
	pIdxEnd = buffer->IdxBuffer + buffer->IdxSize - 1;
	//Set idxSct
	pIHeadTmp->Data = datas;
	pIHeadTmp->Len = len_1;
	pIHeadTmp->Addr = addr | 0x01;
	pIHeadTmp->Reg = reg;

	//DISABLE IT
	buffer->I2Cx->CR2 &= ~(I2C_CR2_ITERREN | I2C_CR2_ITEVTEN);

	//Set Head
	if (pIHeadTmp < pIdxEnd)
		buffer->pIHead = pIHeadTmp + 1;
	else
		buffer->pIHead = buffer->IdxBuffer;
	if (buffer->pIHead == buffer->pITail)
		buffer->IdxEmpty = 0;

	//Enable EV ER IT
	I2C_TransEnable(buffer);

	//ENABLE IT
	buffer->I2Cx->CR2 |= (I2C_CR2_ITERREN | I2C_CR2_ITEVTEN);

	return SUCCESS;
}

#define IS_EVENT(sr, evt) ((sr & evt) == evt)
//Buffer handler
void I2C_BufsHandler(I2C_BufferTypeDef *buffer)
{
	I2C_IdxBuffer *idxNow = buffer->pITail;
	I2C_TypeDef *I2Cx = buffer->I2Cx;
	uint8_t *pData;
	uint32_t SR, SR2;

	buffer->ReTimes = 0;

	//Get flags
	SR = I2Cx->SR1;
	SR2 = I2Cx->SR2;
	SR2 = SR2 << 16;
	SR = (SR | SR2);

	//SB, Send Addr.
	if (IS_EVENT(SR, I2C_EVENT_MASTER_MODE_SELECT))
	{
		//Write:0xFE 0xFE;Read:0xFE 0xFF
		I2Cx->DR = idxNow->Addr & ((I2Cx->CR2 & I2C_CR2_ITBUFEN) ? 0xFE : 0xFF);
		I2Cx->CR2 |= I2C_CR2_ITBUFEN;
		//Reset pData
		buffer->pData = idxNow->Data;
	}
	//ADDR, Send Reg.
	else if (IS_EVENT(SR, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		I2Cx->DR = idxNow->Reg;
		//Close ITBUFEN if is ReadIdx
		if (idxNow->Addr & 0x01)
			I2Cx->CR2 &= ~I2C_CR2_ITBUFEN;
	}
	//BTF, Restart
	else if (IS_EVENT(SR, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if (idxNow->Addr & 0x01)
			//Read Mode
			I2Cx->CR1 |= I2C_CR1_START; //Restart
		else
		//Write Mode
		{
			//pData add
			buffer->pData++;
			if (buffer->pData >= buffer->DataBuffer + buffer->DataSize)
				buffer->pData = buffer->DataBuffer;
			//Set pDTail
			buffer->pDTail = pData;
			if (buffer->pDTail == buffer->pDHead)
				buffer->DataEmpty = 1;
			//Next idxSct
			buffer->pITail++;
			if (buffer->pITail >= buffer->IdxBuffer + buffer->IdxSize)
				buffer->pITail = buffer->IdxBuffer;
			//Is Empty?
			if (buffer->pITail == buffer->pIHead)
			{
				buffer->IdxEmpty = 1;
				I2Cx->CR1 |= I2C_CR1_STOP; //Stop
			}
			else
			{
				I2Cx->CR1 |= I2C_CR1_START; //Restart
			}
			I2Cx->CR2 |= I2C_CR2_ITBUFEN;
		}
		I2Cx->DR = 0x00;
	}
	//TXE, Write next data
	else if (IS_EVENT(SR, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
	{
		pData = buffer->pData;
		//Write data
		I2Cx->DR = *pData;
		//Last one?
		if ((pData < idxNow->Data) ? (pData + buffer->DataSize - idxNow->Data) : (pData - idxNow->Data) >= idxNow->Len)
		{
			//Close ITBUFEN
			I2Cx->CR2 &= ~I2C_CR2_ITBUFEN;
		}
		else
		{
			//pData add
			pData++;
			if (pData >= buffer->DataBuffer + buffer->DataSize)
				pData = buffer->DataBuffer;
			buffer->pData = pData;
		}
	}
	//ADDR, Receive datas
	else if (IS_EVENT(SR, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
	{
		//I2Cx->CR2 |= I2C_CR2_ITBUFEN;
		//If only one to receive
		if (idxNow->Len == 0)
		{
			I2Cx->CR1 &= ~I2C_CR1_ACK;
			//Count next idxSct
			idxNow++;
			if (idxNow >= buffer->IdxBuffer + buffer->IdxSize)
				idxNow = buffer->IdxBuffer;
			//Is Last one?
			if (idxNow == buffer->pIHead)
			{
				I2Cx->CR1 |= I2C_CR1_STOP; //Stop
			}
			else
			{
				I2Cx->CR1 |= I2C_CR1_START; //Restart
			}
			/* * * * * * * * * * * * * * * * * *
			 * WARNING:idxNow has been change  *
			 * * * * * * * * * * * * * * * * * */
		}
		else
		{
			I2Cx->CR1 |= I2C_CR1_ACK;
		}

		//I2Cx->CR1 |= I2C_CR1_STOP;
	}
	//RXNE, Receiving,LastData,OverReceive
	else if (IS_EVENT(SR, I2C_EVENT_MASTER_BYTE_RECEIVED))
	{
		pData = buffer->pData;
		//Save data
		*pData = I2Cx->DR;

		if (pData == idxNow->Data + idxNow->Len - 1) //This is receive so its OK
		{
			//Last one
			I2Cx->CR1 &= ~I2C_CR1_ACK;
			//Count next idxSct
			idxNow++;
			if (idxNow >= buffer->IdxBuffer + buffer->IdxSize)
				idxNow = buffer->IdxBuffer;
			//Is Last one?
			if (idxNow == buffer->pIHead)
			{
				I2Cx->CR1 |= I2C_CR1_STOP; //Stop
			}
			else
			{
				I2Cx->CR1 |= I2C_CR1_START; //Restart
			}
		}
		else if (pData == idxNow->Data + idxNow->Len)
		{
			//Count next idxSct
			idxNow = buffer->pITail + 1;
			if (idxNow >= buffer->IdxBuffer + buffer->IdxSize)
				idxNow = buffer->IdxBuffer;
			buffer->pITail = idxNow;
			//Set empty
			if (buffer->pITail == buffer->pIHead)
				buffer->IdxEmpty = 1;
		}
		/* * * * * * * * * * * * * * * * * *
		 * WARNING:idxNow has been change  *
		 * * * * * * * * * * * * * * * * * */
		buffer->pData++;
	}
}
//Auto restart handle
void I2C_AutoStartHandle(I2C_BufferTypeDef *buffer)
{
	I2C_TypeDef *I2Cx = buffer->I2Cx;
	GPIO_TypeDef *GPIO_SCL;
	GPIO_TypeDef *GPIO_SDA;
	uint16_t PIN_SCL;
	uint16_t PIN_SDA;
	GPIO_InitTypeDef GPIO_InitStructure;
	if (I2Cx == I2C1)
	{
		GPIO_SCL = GPIO_I2C1_SCL;
		PIN_SCL = PIN_I2C1_SCL;
		GPIO_SDA = GPIO_I2C1_SDA;
		PIN_SDA = PIN_I2C1_SDA;
	}
	else if (I2Cx == I2C2)
	{
		GPIO_SCL = GPIO_I2C2_SCL;
		PIN_SCL = PIN_I2C2_SCL;
		GPIO_SDA = GPIO_I2C2_SDA;
		PIN_SDA = PIN_I2C2_SDA;
	}
	if (!buffer->I2Cx->CR2 & I2C_CR2_ITEVTEN)
		return;
	//ReInit
	if (buffer->ReTimes >= I2C_MAX_RETIMES)
	{
		if (!(GPIO_SCL->IDR & PIN_SCL))
			I2Cx->CR1 |= I2C_CR1_SWRST;
		else if (!(GPIO_SDA->IDR & PIN_SDA))
		{
			I2Cx->CR1 &= ~I2C_CR1_PE;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Pin = PIN_SCL;
			GPIO_Init(GPIO_SCL, &GPIO_InitStructure);
			GPIO_InitStructure.GPIO_Pin = PIN_SDA;
			GPIO_Init(GPIO_SDA, &GPIO_InitStructure);

			while(!(GPIO_SDA->IDR & PIN_SDA))
			{
				GPIO_SCL->BRR = PIN_SCL;
				GPIO_SDA->BRR = PIN_SDA;
				GPIO_SCL->BSRR = PIN_SCL;
				GPIO_SDA->BSRR = PIN_SDA;
			}
		}
		buffer->ReTimes = 0;
	}
	if (I2Cx->SR2 & I2C_SR2_BUSY || !(GPIO_SDA->IDR & PIN_SDA))
		buffer->ReTimes++;

	//If is empty
	if (buffer->pIHead == buffer->pITail && buffer->IdxEmpty)
		return;
	//If is Busy Or Starting/Stoping
	if (I2Cx->SR2 & I2C_SR2_BUSY || I2Cx->CR1 & (I2C_CR1_START | I2C_CR1_STOP))
		return;

	//Start I2C
	I2C2->CR2 |= (I2C_CR2_ITBUFEN | I2C_CR2_ITEVTEN | I2C_CR2_ITERREN);
	I2C2->CR1 |= I2C_CR1_START;
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
