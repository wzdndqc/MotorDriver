<center><h1>����Ӳ������</h1></center>

# IO�����

IO | ����1 | ����2
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

# �������-motor

## ����

������������ֱ�����ٵ������������4�������

## �ļ�

[motor.h](./motor.h)

[motor.c](./motor.c)

## ����

�������趨���M1Ϊ��ת50%ת�٣�M2Ϊ��ת50%ת��, M3ɲ����M4����ͣ���ĳ���

```
void main(void)
{
	//��ʼ��
	Motor_Config();

	//ת������
	Motor_Output(1, MOTOR_PERIOD / 2);	//M1
	Motor_Output(2, -MOTOR_PERIOD / 2);	//M2
	Motor_Output(3, 0);			//M3
	//M4
	Motor_Output(4, 0);
	Motor_Stop(4);

	//��ѭ��
	while(1);
}
```
�������趨���M2Ϊ����ͣ�����ⲿ���ƣ���ͬʱ��ȡ����������ֵ�ĳ���

```
uint16_t value_M2_encoder;
void main(void)
{
	//��ʼ��
	Motor_Config();
	Motor_Encoder_Config();

	//ת������
	Motor_Output(2, 0);
	Motor_Stop(2);

	//��ѭ��
	while(1)
	{
		//��ȡ������ֵ
		value_M2_encoder = Motor_GetStep(2);
	}
}
```

# �������-servo

## ����

8·PWM�������������TIM8���4·��������Ҫ�������ʱ����TIM1��������4·��

## �ļ�

[servo.h](./servo.h)

[servo.c](./servo.c)

# �������-A4988

## ����

����A4988�������߲���������������ӿڡ�

# ��������-uart

## ����

USART2��USAER3��Ϊ2·ȫ˫������ʹ�á�

## �ļ�

[uart.h](./uart.h)

[uart.c](./uart.c)

# ADC�ӿ�

## ����

˫·��λ��ADC�ӿ�ADC_IN8~9λ��PB0~1����3.3v��Դ��

# DAC�ӿ�

## ����

Ԥ��˫·DAC�ӿ�DAC_OUT1~2λ��PA4~5��

# I2C�ӿ�

## ����

Ĭ��I2C�ӿ�ΪI2C2�����ض�ӻ���MPU6050��OLED-0.96��

# SPI�ӿ�

## ����

Ĭ��SPI�ӿ�ΪSPI2������2.4G����ģ��NRF24L01����Ҫע��NRF24L01����������USART3���ã�ͨ������ñP10�л���

# CAN����

## ����

����CAN�շ���������ʹ��CAN���ߡ�
