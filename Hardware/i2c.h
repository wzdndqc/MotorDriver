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

//Fun
void I2C_Config(void);
uint8_t I2C_TestWhileSTD(void);
uint8_t I2C_TestWhileREG(void);

#endif
