#ifndef __JY61_H
#define __JY61_H

#define JY61_SAVE 0x00
#define JY61_CALSW 0x01
#define JY61_RSW 0x02
#define JY61_RRATE 0x03
#define JY61_BAUD 0x04
#define JY61_AXOFFSET 0x05
#define JY61_AYOFFSET 0x06
#define JY61_AZOFFSET 0x07
#define JY61_GXOFFSET 0x08
#define JY61_GYOFFSET 0x09
#define JY61_GZOFFSET 0x0a
#define JY61_HXOFFSET 0x0b
#define JY61_HYOFFSET 0x0c
#define JY61_HZOFFSET 0x0d
#define JY61_D0MODE 0x0e
#define JY61_D1MODE 0x0f
#define JY61_D2MODE 0x10
#define JY61_D3MODE 0x11
#define JY61_D0PWMH 0x12
#define JY61_D1PWMH 0x13
#define JY61_D2PWMH 0x14
#define JY61_D3PWMH 0x15
#define JY61_D0PWMT 0x16
#define JY61_D1PWMT 0x17
#define JY61_D2PWMT 0x18
#define JY61_D3PWMT 0x19
#define JY61_IICADDR 0x1a
#define JY61_LEDOFF 0x1b
#define JY61_GPSBAUD 0x1c

#define JY61_YYMM 0x30
#define JY61_DDHH 0x31
#define JY61_MMSS 0x32
#define JY61_MS 0x33
#define JY61_AX 0x34
#define JY61_AY 0x35
#define JY61_AZ 0x36
#define JY61_GX 0x37
#define JY61_GY 0x38
#define JY61_GZ 0x39
#define JY61_HX 0x3a
#define JY61_HY 0x3b
#define JY61_HZ 0x3c
#define JY61_Roll 0x3d
#define JY61_Pitch 0x3e
#define JY61_Yaw 0x3f
#define JY61_TEMP 0x40
#define JY61_D0Status 0x41
#define JY61_D1Status 0x42
#define JY61_D2Status 0x43
#define JY61_D3Status 0x44
#define JY61_PressureL 0x45
#define JY61_PressureH 0x46
#define JY61_HeightL 0x47
#define JY61_HeightH 0x48
#define JY61_LonL 0x49
#define JY61_LonH 0x4a
#define JY61_LatL 0x4b
#define JY61_LatH 0x4c
#define JY61_GPSHeight 0x4d
#define JY61_GPSYAW 0x4e
#define JY61_GPSVL 0x4f
#define JY61_GPSVH 0x50

#define JY61_DIO_MODE_AIN 0
#define JY61_DIO_MODE_DIN 1
#define JY61_DIO_MODE_DOH 2
#define JY61_DIO_MODE_DOL 3
#define JY61_DIO_MODE_DOPWM 4
#define JY61_DIO_MODE_GPS 5

struct JY61_STime
{
	unsigned char ucYear;
	unsigned char ucMonth;
	unsigned char ucDay;
	unsigned char ucHour;
	unsigned char ucMinute;
	unsigned char ucSecond;
	unsigned short usMiliSecond;
};
struct JY61_SAcc
{
	short a[3];
	short T;
};
struct JY61_SGyro
{
	short w[3];
	short T;
};
struct JY61_SAngle
{
	short Angle[3];
	short T;
};
struct JY61_SMag
{
	short h[3];
	short T;
};

struct JY61_SDStatus
{
	short sDStatus[4];
};

struct JY61_SPress
{
	long lPressure;
	long lAltitude;
};

struct JY61_SLonLat
{
	long lLon;
	long lLat;
};

struct JY61_SGPSV
{
	short sGPSHeight;
	short sGPSYaw;
	long lGPSVelocity;
};

extern struct JY61_STime JY61_stcTime;
extern struct JY61_SAcc JY61_stcAcc;
extern struct JY61_SGyro JY61_stcGyro;
extern struct JY61_SAngle JY61_stcAngle;
extern struct JY61_SMag JY61_stcMag;
extern struct JY61_SDStatus JY61_stcDStatus;
extern struct JY61_SPress JY61_stcPress;
extern struct JY61_SLonLat JY61_stcLonLat;
extern struct JY61_SGPSV JY61_stcGPSV;

//接收完成Flag
extern unsigned char JY61_FlagRx;

//CopeSerialData为串口中断调用函数，串口每收到一个数据，调用一次这个函数。
void JY61_RxData(unsigned char ucData);

#endif
