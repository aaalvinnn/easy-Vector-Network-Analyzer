#include "ad8302.h"
#include "math.h"
/**
 * @brief ad8032返回两个输入信号的相位差，需要ADC去采，这里选择片内ADC
 * 
 * @param adc_result 
 * @return int 
 */
double ad8302_getPhase(uint16_t adc_result)
{
    double delta = 10.000 * 4095.000 / 3300.000; // 每10mv对应1°,片内ADC精度为12位
    return (180 - ((double)adc_result / delta));
}

/**
 * @brief ad8032返回两个输入信号的功率分贝值之差，需要ADC去采
 * 
 * @param adc_result 
 * @return int 
 */
double ad8302_getAmp(uint16_t adc_result)
{
    double delta = 30.000 * 4095.000 / 3300.000;// 每30mv对应1db，片内精度为12位
    double db = ((float)adc_result / delta -30);
		double power = pow(10, db / 20);
	  return power;
}

int ad8302_mean(uint16_t *adc_result)
{
	int mean=0;
	for (int i=0;i<10;i++)
	{
		mean += adc_result[i];
	}
	return mean/10;
}