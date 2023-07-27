#ifndef __AD9854_H
#define __AD9854_H	 
#include "main.h"


#define AD9854_RST_1    			HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, 1)   //AD9854复位端口
#define AD9854_RST_0    			HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, 0)
#define AD9854_UDCLK_1  				HAL_GPIO_WritePin(UCLK_GPIO_Port, UCLK_Pin, 1)   //AD9854更新时钟
#define AD9854_UDCLK_0  				HAL_GPIO_WritePin(UCLK_GPIO_Port, UCLK_Pin, 0)
#define AD9854_WR_1     				HAL_GPIO_WritePin(WD_GPIO_Port, WD_Pin, 1)  //AD9854写使能，低有效
#define AD9854_WR_0     				HAL_GPIO_WritePin(WD_GPIO_Port, WD_Pin, 0)
#define AD9854_RD_1     				HAL_GPIO_WritePin(RD_GPIO_Port, RD_Pin, 1)   //AD9854读使能，低有效
#define AD9854_RD_0     				HAL_GPIO_WritePin(RD_GPIO_Port, RD_Pin, 0)
#define AD9854_OSK_1   					HAL_GPIO_WritePin(OSK_GPIO_Port, OSK_Pin, 1)   //AD9854 OSK控制端
#define AD9854_OSK_0   					HAL_GPIO_WritePin(OSK_GPIO_Port, OSK_Pin, 0)
#define AD9854_FSK_BPSK_HOLD_1  HAL_GPIO_WritePin(FSK_GPIO_Port, FSK_Pin, 1)   //AD9854 FSK,BPSK,HOLD控制脚，即AD9854芯片29脚
#define AD9854_FSK_BPSK_HOLD_0  HAL_GPIO_WritePin(FSK_GPIO_Port, FSK_Pin, 0)


#define AUTO 		1//自动扫频
#define MANUAL	0//FSK控制脚控制扫频

#define uint  unsigned int
#define uchar unsigned char
#define ulong unsigned long
	
//**************************以下部分为函数定义********************************

void AD9854_IO_Init(void);								//AD9854需要用到的IO口初始化
void AD9854_WR_Byte(uint32_t addr,uint32_t dat);		//AD9854并行口写数据
void Freq_convert(long Freq);	 						//正弦信号频率数据转换
void Freq_double_convert(double Freq);		//正弦信号频率数据转换,入口参数为double，可使信号的频率更精确

void AD9854_InitSingle(void);	      	  					//AD9854点频模式初始化
void AD9854_SetSine(ulong Freq,uint Shape);	 			//AD9854正弦波产生程序,频率为整数
void AD9854_SetSine_double(double Freq,uint Shape);//AD9854正弦波产生程序,频率可为小数

void AD9854_InitFSK(void);												//AD9854的FSK模式初始化
void AD9854_SetFSK(ulong Freq1,ulong Freq2);			//AD9854的FSK参数设置

void AD9854_InitRFSK(uchar autoSweepEn);					//AD9854的RFSK模式初始化 线性调频模式即扫频	 
void AD9854_SetRFSK(ulong Freq_Low,ulong Freq_High,ulong Freq_Up_Down,ulong FreRate);	//AD9854的RFSK扫频参数设置	

void AD9854_InitChirp(void);											//AD9854的Chirp模式初始化
void AD9854_SetChirp(ulong Freq,uint Shape,ulong Freq_Up_Down,ulong FreRate);//AD9854的Chirp扫频参数设置	

void AD9854_InitBPSK(void);												//AD9854的BPSK模式初始化			  
void AD9854_SetBPSK(ulong Freq,uint Shape,uint Phase1,uint Phase2);			//AD9854的BPSK参数设置

void AD9854_InitOSK(void);					 							//AD9854的OSK模式初始化
void AD9854_SetOSK(ulong Freq,uchar RateShape);		//AD9854的OSK参数设置

#endif

