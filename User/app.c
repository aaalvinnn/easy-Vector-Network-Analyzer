#include "app.h"
#include "adc.h"
#include "AD9854.h"
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
uint16_t adc_buf[ADCLENGTH];
volatile uint8_t adc1_dma_flag = 0;		// volatile ����Ҫ��������Ȼ�Ῠ����whileѭ����
volatile uint8_t adc2_dma_flag = 0;
ADC1256 adc1256;

/*---------------------- FUNCTION ---------------------------*/
/* ѡ��ADCͨ�� */
// void changeAdcChannel(ADC_HandleTypeDef *hadc, uint32_t channel);

/** @brief ����ADC�ɼ�-Ƭ��ADC */
void startAdc(uint16_t* _adc_buf, ADC_HandleTypeDef* hadc)
{
  if(hadc == &hadc1)
  {
    HAL_TIM_Base_Start_IT(&htim2);
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)_adc_buf, ADCLENGTH); 
		while(!adc1_dma_flag);    // �ȴ�DMA�������
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

/** @brief ����ADC����-ADS1256 ������1kHz
  *	@retval �޷���ֵ�����ı�ȫ�ֱ���adc1256.adc1256_buf
	*/
void start_Adc_1256(void)
{
	HAL_TIM_Base_Start_IT(&htim3);
	while(!adc1256.flag);
	// for (int i = 0;i < NUMS; i++)	printf("%d\r\n", adc1256_buf[i]);
	adc1256.flag = 0;	// ���ת����ɱ�־λ
}

/** @brief �Զ�ɨƵ
  * 
  * @retval	void 
	*/
void start_FSK(void)
{
	AD9854_FSK_BPSK_HOLD_1;		// FSK = 1: �ӵ�Ƶ���ƵɨƵ; FSK = 0: �Ӹ�Ƶ���ƵɨƵ
	AD9854_InitRFSK(MANUAL);									//AUTO,�Զ�ɨƵ��������ӿ��ƽſ��ƣ�MANUAL,�ֶ�ɨƵ����AD9854_FSK_BPSK_HOLD���ſ���ɨƵ
	AD9854_SetRFSK(500000,600000,500,299999);	//ɨƵ����0.5Mhz������120Mhz������500Khz��Ƶ����1ms ����ads1256���������Ӧ����һ��ɨ(120-0.5) / 0.5 = 239 ����
}

/**
 * @brief ����S21����
 * @param void
 * @retval void
 */
void measure_S21(void)
{

}

/**
 * @brief ����S11����������ɨƵ�㷨
 * @param void
 * @retval void
 */
void measure_S11(void)
{
	// 1.�����ź���coswt���
	start_FSK();	// ����AD9854�Զ�ɨƵģʽ
	start_Adc_1256();	// ��ADS1256����

}