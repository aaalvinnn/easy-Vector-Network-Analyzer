#ifndef _USER_H
#define	_USER_H
#include "main.h"

#define ADCLENGTH 500 // ADCת�������С

extern uint16_t adc_buf[ADCLENGTH];


void startAdc(uint16_t* _adc_buf, ADC_HandleTypeDef* hadc);

#endif