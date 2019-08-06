#include "JY61.h"
#include <string.h>

struct JY61_STime JY61_stcTime;
struct JY61_SAcc JY61_stcAcc;
struct JY61_SGyro JY61_stcGyro;
struct JY61_SAngle JY61_stcAngle;
struct JY61_SMag JY61_stcMag;
struct JY61_SDStatus JY61_stcDStatus;
struct JY61_SPress JY61_stcPress;
struct JY61_SLonLat JY61_stcLonLat;
struct JY61_SGPSV JY61_stcGPSV;

unsigned char JY61_FlagRx = 0;

//CopeSerialData为串口中断调用函数，串口每收到一个数据，调用一次这个函数。
void JY61_RxData(unsigned char ucData)
{
	static unsigned char ucRxBuffer[250];
	static unsigned char ucRxCnt = 0;

	ucRxBuffer[ucRxCnt++] = ucData;
	if (ucRxBuffer[0] != 0x55) //数据头不对，则重新开始寻找0x55数据头
	{
		ucRxCnt = 0;
		return;
	}
	if (ucRxCnt < 11)
	{
		return;
	} //数据不满11个，则返回
	else
	{
		switch (ucRxBuffer[1])
		{
		case 0x50:
			memcpy(&JY61_stcTime, &ucRxBuffer[2], 8);
			break; //memcpy为编译器自带的内存拷贝函数，需引用"string.h"，将接收缓冲区的字符拷贝到数据共同体里面，从而实现数据的解析。
		case 0x51:
			memcpy(&JY61_stcAcc, &ucRxBuffer[2], 8);
			break;
		case 0x52:
			memcpy(&JY61_stcGyro, &ucRxBuffer[2], 8);
			break;
		case 0x53:
			memcpy(&JY61_stcAngle, &ucRxBuffer[2], 8);
			break;
		case 0x54:
			memcpy(&JY61_stcMag, &ucRxBuffer[2], 8);
			break;
		case 0x55:
			memcpy(&JY61_stcDStatus, &ucRxBuffer[2], 8);
			break;
		case 0x56:
			memcpy(&JY61_stcPress, &ucRxBuffer[2], 8);
			break;
		case 0x57:
			memcpy(&JY61_stcLonLat, &ucRxBuffer[2], 8);
			break;
		case 0x58:
			memcpy(&JY61_stcGPSV, &ucRxBuffer[2], 8);
			break;
		}
		ucRxCnt = 0;
		JY61_FlagRx = 1;
	}
}
