#ifndef _USER_H
#define	_USER_H
#include "main.h"
#include "stdio.h"

#define ADCLENGTH 500 // ADC转换数组大小
#define NUMS 200			// 幅频/相频相应曲线点数

extern uint16_t adc_buf[ADCLENGTH];


void startAdc(uint16_t* _adc_buf, ADC_HandleTypeDef* hadc);
void start_Adc_1256(void);
void start_FSK(void);

#endif