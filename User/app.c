#include "app.h"

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern TIM_HandleTypeDef htim2;
uint16_t adc_buf[ADCLENGTH];
volatile uint8_t adc1_dma_flag = 0;		// volatile ����Ҫ��������Ȼ�Ῠ����whileѭ����
volatile uint8_t adc2_dma_flag = 0;

/*---------------------- FUNCTION ---------------------------*/
/* ѡ��ADCͨ�� */
// void changeAdcChannel(ADC_HandleTypeDef *hadc, uint32_t channel);

/* ����ADC�ɼ� */
void startAdc(uint16_t* _adc_buf, ADC_HandleTypeDef* hadc)
{
  if(hadc == &hadc1)
  {
    HAL_TIM_Base_Start_IT(&htim2);
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)_adc_buf, ADCLENGTH); 
		while(!adc1_dma_flag);
    HAL_TIM_Base_Stop_IT(&htim2);
		adc1_dma_flag = 0;
		return;
  }

  else if(hadc == &hadc2)
  {
    HAL_TIM_Base_Start_IT(&htim2);
    HAL_ADC_Start_DMA(&hadc2, (uint32_t *)_adc_buf, ADCLENGTH);
		while(!adc2_dma_flag);
    HAL_TIM_Base_Stop_IT(&htim2);
		adc2_dma_flag = 0;
		return;
  }
	return;
}
