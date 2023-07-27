#ifndef __AD9854_H
#define __AD9854_H	 
#include "main.h"


#define AD9854_RST_1    			HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, 1)   //AD9854��λ�˿�
#define AD9854_RST_0    			HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, 0)
#define AD9854_UDCLK_1  				HAL_GPIO_WritePin(UCLK_GPIO_Port, UCLK_Pin, 1)   //AD9854����ʱ��
#define AD9854_UDCLK_0  				HAL_GPIO_WritePin(UCLK_GPIO_Port, UCLK_Pin, 0)
#define AD9854_WR_1     				HAL_GPIO_WritePin(WD_GPIO_Port, WD_Pin, 1)  //AD9854дʹ�ܣ�����Ч
#define AD9854_WR_0     				HAL_GPIO_WritePin(WD_GPIO_Port, WD_Pin, 0)
#define AD9854_RD_1     				HAL_GPIO_WritePin(RD_GPIO_Port, RD_Pin, 1)   //AD9854��ʹ�ܣ�����Ч
#define AD9854_RD_0     				HAL_GPIO_WritePin(RD_GPIO_Port, RD_Pin, 0)
#define AD9854_OSK_1   					HAL_GPIO_WritePin(OSK_GPIO_Port, OSK_Pin, 1)   //AD9854 OSK���ƶ�
#define AD9854_OSK_0   					HAL_GPIO_WritePin(OSK_GPIO_Port, OSK_Pin, 0)
#define AD9854_FSK_BPSK_HOLD_1  HAL_GPIO_WritePin(FSK_GPIO_Port, FSK_Pin, 1)   //AD9854 FSK,BPSK,HOLD���ƽţ���AD9854оƬ29��
#define AD9854_FSK_BPSK_HOLD_0  HAL_GPIO_WritePin(FSK_GPIO_Port, FSK_Pin, 0)


#define AUTO 		1//�Զ�ɨƵ
#define MANUAL	0//FSK���ƽſ���ɨƵ

#define uint  unsigned int
#define uchar unsigned char
#define ulong unsigned long
	
//**************************���²���Ϊ��������********************************

void AD9854_IO_Init(void);								//AD9854��Ҫ�õ���IO�ڳ�ʼ��
void AD9854_WR_Byte(uint32_t addr,uint32_t dat);		//AD9854���п�д����
void Freq_convert(long Freq);	 						//�����ź�Ƶ������ת��
void Freq_double_convert(double Freq);		//�����ź�Ƶ������ת��,��ڲ���Ϊdouble����ʹ�źŵ�Ƶ�ʸ���ȷ

void AD9854_InitSingle(void);	      	  					//AD9854��Ƶģʽ��ʼ��
void AD9854_SetSine(ulong Freq,uint Shape);	 			//AD9854���Ҳ���������,Ƶ��Ϊ����
void AD9854_SetSine_double(double Freq,uint Shape);//AD9854���Ҳ���������,Ƶ�ʿ�ΪС��

void AD9854_InitFSK(void);												//AD9854��FSKģʽ��ʼ��
void AD9854_SetFSK(ulong Freq1,ulong Freq2);			//AD9854��FSK��������

void AD9854_InitRFSK(uchar autoSweepEn);					//AD9854��RFSKģʽ��ʼ�� ���Ե�Ƶģʽ��ɨƵ	 
void AD9854_SetRFSK(ulong Freq_Low,ulong Freq_High,ulong Freq_Up_Down,ulong FreRate);	//AD9854��RFSKɨƵ��������	

void AD9854_InitChirp(void);											//AD9854��Chirpģʽ��ʼ��
void AD9854_SetChirp(ulong Freq,uint Shape,ulong Freq_Up_Down,ulong FreRate);//AD9854��ChirpɨƵ��������	

void AD9854_InitBPSK(void);												//AD9854��BPSKģʽ��ʼ��			  
void AD9854_SetBPSK(ulong Freq,uint Shape,uint Phase1,uint Phase2);			//AD9854��BPSK��������

void AD9854_InitOSK(void);					 							//AD9854��OSKģʽ��ʼ��
void AD9854_SetOSK(ulong Freq,uchar RateShape);		//AD9854��OSK��������

#endif

