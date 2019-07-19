<center><h1>板载硬件驱动</h1></center>

# IO分配表

IO | 功能1 | 功能2
-|-|-|
PA0 | M4_EA |
PA1 | M4_EB |
PA2 | USART2_TX |
PA3 | USART2_RX |
PA4 | DAC_OUT1 |
PA5 | DAC_OUT2 |
PA6 | M2_EA |
PA7 | M2_EB |
PA8 | M1_PWM | T1_SERVO1
PA9 | M2_PWM | T1_SERVO2
PA10| M3_PWM | T1_SERVO3
PA11| M4_PWM | T1_SERVO4
PA12| KEY0 |
PA13| SWDIO |
PA14| SWCLK |
PA15| M1_EA |
PB0 | ADC_IN8 |
PB1 | ADC_IN9 |
PB2 | BOOT1 |
PB3 | M1_EB |
PB4 | M2_N1 |
PB5 | M2_N2 |
PB6 | M3_EA |
PB7 | M3_EB |
PB8 | CAN_RX |
PB9 | CAN_TX |
PB10| I2C2_SCL |
PB11| I2C2_SDA |
PB12| SPI2_NSS |
PB13| SPI2_SCK |
PB14| SPI2_MISO |
PB15| SPI2_MOSI |
PC0 | M3_N1 |
PC1 | M3_N2 |
PC2 | M4_N1 |
PC3 | M4_N2 |
PC4 | A4988_DIR |
PC5 | A4988_STEP |
PC6 | T8_SERVO1 |
PC7 | T8_SERVO2 |
PC8 | T8_SERVO3 |
PC9 | T8_SERVO4 |
PC10| NRF_CE | USART3_TX
PC11| NRF_IRQ| USART3_RX
PC12| M1_N1 |
PC13| LED0 |
PC14| OSC32_IN |
PC15| OSC32_OUT |
PD0 | OSC_IN |
PD1 | OSC_OUT |
PD2 | M1_N2 |

# 电机驱动-motor

## 描述

带正交编码器直流减速电机的驱动，共4个电机。

## 文件

[motor.h](./motor.h)

[motor.c](./motor.c)

## 例程

以下是设定电机M1为正转50%转速，M2为反转50%转速, M3刹车，M4自由停车的程序：

```
void main(void)
{
	//初始化
	Motor_Config();

	//转速设置
	Motor_Output(1, MOTOR_PERIOD / 2);	//M1
	Motor_Output(2, -MOTOR_PERIOD / 2);	//M2
	Motor_Output(3, 0);			//M3
	//M4
	Motor_Output(4, 0);
	Motor_Stop(4);

	//死循环
	while(1);
}
```
以下是设定电机M2为自由停车（外部控制），同时读取编码器返回值的程序：

```
uint16_t value_M2_encoder;
void main(void)
{
	//初始化
	Motor_Config();
	Motor_Encoder_Config();

	//转速设置
	Motor_Output(2, 0);
	Motor_Stop(2);

	//死循环
	while(1)
	{
		//获取编码器值
		value_M2_encoder = Motor_GetStep(2);
	}
}
```

# 舵机驱动-servo

## 描述

8路PWM舵机控制驱动。TIM8输出4路，当不需要电机驱动时可用TIM1控制另外4路。

## 文件

[servo.h](./servo.h)

[servo.c](./servo.c)

# 步进电机-A4988

## 描述

板载A4988两相四线步进电机驱动器及接口。

# 串口驱动-uart

## 描述

USART2、USAER3作为2路全双工串口使用。

## 文件

[uart.h](./uart.h)

[uart.c](./uart.c)

# ADC接口

## 描述

双路电位器ADC接口ADC_IN8~9位于PB0~1，带3.3v电源。

# DAC接口

## 描述

预留双路DAC接口DAC_OUT1~2位于PA4~5。

# I2C接口

## 描述

默认I2C接口为I2C2，板载多从机：MPU6050及OLED-0.96。

# SPI接口

## 描述

默认SPI接口为SPI2，板载2.4G无线模块NRF24L01，需要注意NRF24L01部分引脚与USART3复用，通过跳线帽P10切换。

# CAN总线

## 描述

板载CAN收发器，可以使用CAN总线。
