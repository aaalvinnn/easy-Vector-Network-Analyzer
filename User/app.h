#ifndef _USER_H
#define	_USER_H
#include "main.h"
#include "stdio.h"

#define ADCLENGTH 500 // ADCת�������С
#define NUMS 200			// ��Ƶ/��Ƶ��Ӧ���ߵ���

extern uint16_t adc_buf[ADCLENGTH];


void startAdc(uint16_t* _adc_buf, ADC_HandleTypeDef* hadc);
void start_Adc_1256(void);
void start_FSK(void);

#endif