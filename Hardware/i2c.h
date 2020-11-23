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
#ifndef _I2C_H_
#define _I2C_H_

//Inc
#include "stm32f10x.h"

//Var
//Own address of I2C2
#define I2C2_SLAVE_ADDRESS7 0xA0
//Speed of I2C2
#define I2C2_SPEED 400000
//索引结构体
typedef struct
{
	//器件地址+读/写
	uint8_t Addr;
	//寄存器地址
	uint8_t Reg;
	//数据起始指针
	uint8_t* Data;
	//数据长度-1
	uint8_t Len;
} I2C_IdxBuffer;
//缓冲区结构体
typedef struct
{
	//缓冲区所用的I2C
	I2C_TypeDef *I2Cx;
	//索引缓冲区
	I2C_IdxBuffer *IdxBuffer;
	//索引缓冲区大小
	uint16_t IdxSize;
	//索引缓冲区队头指针
	I2C_IdxBuffer *pIHead;
	//索引缓冲区队尾指针
	I2C_IdxBuffer *pITail;
	//索引缓冲区空标志位
	uint8_t IdxEmpty;
	//数据缓冲区
	uint8_t *DataBuffer;
	//数据缓冲区大小
	uint16_t DataSize;
	//数据缓冲区队头指针
	uint8_t *pDHead;
	//数据缓冲区队尾指针
	uint8_t *pDTail;
	//数据缓冲区空标志位
	uint8_t DataEmpty;
	//当前数据指针
	uint8_t *pData;
	//死机自动重启计数
	uint8_t ReTimes;
} I2C_BufferTypeDef;
//最大自动复位模块计数(ms maybe)
#define I2C_MAX_RETIMES 20
#define GPIO_I2C2_SDA GPIOB
#define PIN_I2C2_SDA GPIO_IDR_IDR11
#define GPIO_I2C1_SDA GPIOB
#define PIN_I2C1_SDA GPIO_IDR_IDR9//Remap

//Fun
void I2C_Config(void);
//缓冲区初始化函数
void I2C_InitBuffer(I2C_BufferTypeDef *buffer, I2C_TypeDef *I2Cx, I2C_IdxBuffer *idxBufs, uint16_t idxSize, uint8_t *dataBufs, uint16_t dataSize);
//缓冲区写函数
ErrorStatus I2C_WriteBuffer(I2C_BufferTypeDef *buffer, uint8_t addr, uint8_t reg, uint8_t *datas, uint8_t len_1);
//缓冲区读函数
ErrorStatus I2C_ReadMem(I2C_BufferTypeDef *buffer, uint8_t addr, uint8_t reg, uint8_t *datas, uint8_t len_1);
//缓冲区中断服务函数
void I2C_BufsHandler(I2C_BufferTypeDef *buffer);
//缓冲区自动重启服务函数 写入缓冲区后、定时执行
void I2C_AutoStartHandle(I2C_BufferTypeDef *buffer);
void I2C_TransEnable(I2C_BufferTypeDef *buffer);
//Test
uint8_t I2C_TestWhileREG(void);

#endif
