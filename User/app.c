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
ADC1256 adc1256;	// �ɼ�ԭʼ����
RESULT math_result;	// �����ֽ��㷨����Ľ��

/*---------------------- TABLE ---------------------------*/
// У��



/*---------------------- DRIVER ---------------------------*/
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

/*---------------------- FUNCTION ---------------------------*/
/**
 * @brief �����ֽ��㷨��������źŵ�amp��phase
 * 
 * @param raw
 */
void calculate_Amp_Phase(ADC1256 raw)
{
	for(int i=0;i<NUMS;i++)
	{
		// ��Ƶ��Ӧ
		math_result.phase[i] = atan2(raw.adc1256_buf_sin[i],raw.adc1256_buf_cos[i]);
		// ��Ƶ��Ӧ (�������⣬Uz��Uoδ֪)
		math_result.amp[i] = 2.0 * sqrt(raw.adc1256_buf_cos[i]*raw.adc1256_buf_cos[i] + raw.adc1256_buf_sin[i]*raw.adc1256_buf_sin[i]);
	}
	return ;
}

/**
 * @brief ����S21����������ɨƵ�㷨��ֻ�ɾ����˷�������ͨ�˲������ֱ����
 * @param void
 * @retval void
 */
void measure_S21(void)
{
	// 1.��Ƶ�����л�ΪS21�����ź�

	// 2.�����ź���coswt���
	SWITCH_RFC1_TO_RFC2;	// �л�����, �˷�������Ϊcos
	adc1256.channel = COS;
	start_FSK();			// ����AD9854�Զ�ɨƵģʽ
	start_Adc_1256();	// ��ADS1256����

	// 3.�����ź���sinwt���
	SWITCH_RFC1_TO_RF1;	// �л�����, �˷�������Ϊsin
	adc1256.channel = SIN;
	start_FSK();			// ����AD9854�Զ�ɨƵģʽ
	start_Adc_1256();	// ��ADS1256����

	// 4.����ͨ��RLC����Ĵ����źŵ�U�ͦ�
	calculate_Amp_Phase(adc1256);

	return ;
}

/**
 * @brief ����S11����������ɨƵ�㷨
 * @param void
 * @retval void
 */
void measure_S11(void)
{
	// 1.��Ƶ�����л�ΪS11�����ź�

	// 2.�����ź���coswt���
	SWITCH_RFC1_TO_RFC2;	// �л�����, �˷�������Ϊcos
	adc1256.channel = COS;
	start_FSK();			// ����AD9854�Զ�ɨƵģʽ
	start_Adc_1256();	// ��ADS1256����

	// 3.�����ź���sinwt���
	SWITCH_RFC1_TO_RF1;	// �л�����, �˷�������Ϊsin
	adc1256.channel = SIN;
	start_FSK();			// ����AD9854�Զ�ɨƵģʽ
	start_Adc_1256();	// ��ADS1256����

	// 4.����ͨ��RLC����Ĵ����źŵ�U�ͦ�
	calculate_Amp_Phase(adc1256);

	return ;
}

/*---------------------- GUI ---------------------------*/
/**
 * @brief ��ӡ��Ƶ����Ƶ��Ӧ����
 * 
 */
void gui(void)
{
	for(int i=0;i<NUMS;i++)
	{
		// ��Ƶ����
		printf("add s0.id,0,%d\xff\xff\xff",(int)math_result.amp[i]);
		// ��Ƶ����
		printf("add s1.id,0,%d\xff\xff\xff",(int)math_result.phase[i]);
	}
	return ;
}