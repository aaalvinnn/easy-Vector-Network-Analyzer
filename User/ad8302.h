#ifndef _AD8302_H
#define _AD8302_H

#include "stdio.h"
#include "main.h"

double ad8302_getPhase(uint16_t adc_result);

double ad8302_getAmp(uint16_t adc_result);

int ad8302_mean(uint16_t *adc_result);

#endif 