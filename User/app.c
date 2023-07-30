/**
 * @file app.c
 * @author zsw (aaalvinnn@foxmail.com)
 * @brief ���ܺ�����ؽӿ�
 * @version 0.1
 * @date 2023-07-30
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "app.h"
#include "adc.h"
#include "AD9854.h"
#include "math.h"
#include "gui.h"

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
uint16_t adc_buf[ADCLENGTH];
volatile uint8_t adc1_dma_flag = 0;		// volatile ����Ҫ��������Ȼ�Ῠ����whileѭ����
volatile uint8_t adc2_dma_flag = 0;
ADC1256 adc1256;	// �ɼ�ԭʼ����
RESULT math_result;	// �����ֽ��㷨����Ľ��
SELFCALIBRATION self_calibration;	// ��У��ϵ��

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
	AD9854_SetRFSK(500000,120000000,FRESTEP,299999);	//ɨƵ����0.5Mhz������120Mhz������500Khz��Ƶ����1ms ����ads1256���������Ӧ����һ��ɨ(120-0.5) / 0.5 = 239 ����
}

/*---------------------- FUNCTION ---------------------------*/
/**
 * @brief �����ֽ��㷨��������źŵ�amp��phase
 * @param raw �ɼ�����ԭʼ����
 * @param calibration ��У��ϵ��
 */
void calculate_Amp_Phase()
{
	for(int i=0;i<NUMS;i++)
	{
		// ��Ƶ��Ӧ
		math_result.phase[i] = atan2(adc1256.adc1256_buf_sin[i] / self_calibration.calibration_sin[i],adc1256.adc1256_buf_cos[i] / self_calibration.calibration_cos[i]);
		math_result.phase[i] = math_result.phase[i] / PI * 180.00;	// �л�Ϊ�Ƕȵ�λ
		// ��Ƶ��Ӧ
		math_result.amp[i] = 2.0 * sqrt((adc1256.adc1256_buf_cos[i] / self_calibration.calibration_cos[i]) * (adc1256.adc1256_buf_cos[i] / self_calibration.calibration_cos[i])
										 + 
										 (adc1256.adc1256_buf_sin[i] / self_calibration.calibration_sin[i]) * (adc1256.adc1256_buf_sin[i] / self_calibration.calibration_sin[i]));
		math_result.amp[i] = 20 * log10(math_result.amp[i]);	// �л�Ϊ������λ
	}
	return ;
}

/**
 * @brief ��ȡ��У������
 * @retval void,���ı�ȫ�ֱ���calibration(���������ֽ��źŵķ�ֵ�Լ��˷������˲�����˥��ϵ��)
 */
void self_Calibration(void)
{
	// 1.��Ƶ�����л�Ϊ������У����·
	SWITCH34_CALIBRATION;
	// 2.�ɼ�cosУ��ϵ��
	SWITCH1_COS;	// �л�����, �˷�������Ϊcos
	adc1256.channel = COS;
	start_FSK();			// ����AD9854�Զ�ɨƵģʽ
	start_Adc_1256();	// ��ADS1256����
	// for(int i=0;i<NUMS;i++)	self_calibration.calibration_cos[i] = adc1256.adc1256_buf_cos[i];	// ���ɼ�������浽��У��ϵ��������
	// 3.�ɼ�sinУ��ϵ��
	SWITCH1_SIN;	// �л�����, �˷�������Ϊsin
	adc1256.channel = SIN;
	start_FSK();			// ����AD9854�Զ�ɨƵģʽ
	start_Adc_1256();	// ��ADS1256����
	// for(int i=0;i<NUMS;i++)	self_calibration.calibration_sin[i] = adc1256.adc1256_buf_sin[i];	// ���ɼ�������浽��У��ϵ��������
	// 4.��Ƶ�����л��ز���RLC��������
	SWITCH34_RLC;
	return ;
}

/**
 * @brief �����ýӿ�
 * 
 */
void self_Calibration_Test(void)
{
	for(int i=0;i<NUMS;i++)	
	{
		self_calibration.calibration_cos[i] = 1;
		self_calibration.calibration_sin[i] = 1;
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
	SWITCH2_S21;
	// 2.�����ź���coswt���
	SWITCH1_COS;	// �л�����, �˷�������Ϊcos
	adc1256.channel = COS;
	start_FSK();			// ����AD9854�Զ�ɨƵģʽ
	start_Adc_1256();	// ��ADS1256����
	// for(int i=0;i<NUMS;i++)	printf("%d\r\n",adc1256.adc1256_buf_cos[i]);

	// 3.�����ź���sinwt���
	SWITCH1_SIN;	// �л�����, �˷�������Ϊsin
	adc1256.channel = SIN;
	start_FSK();			// ����AD9854�Զ�ɨƵģʽ
	start_Adc_1256();	// ��ADS1256����
	// for(int i=0;i<NUMS;i++)	printf("%d\r\n",adc1256.adc1256_buf_sin[i]);
	
	// 4.����ͨ��RLC����Ĵ����źŵ�U�ͦ�
	calculate_Amp_Phase();

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
	SWITCH2_S11;
	// 2.�����ź���coswt���
	SWITCH1_COS;	// �л�����, �˷�������Ϊcos
	adc1256.channel = COS;
	start_FSK();			// ����AD9854�Զ�ɨƵģʽ
	start_Adc_1256();	// ��ADS1256����

	// 3.�����ź���sinwt���
	SWITCH1_SIN;	// �л�����, �˷�������Ϊsin
	adc1256.channel = SIN;
	start_FSK();			// ����AD9854�Զ�ɨƵģʽ
	start_Adc_1256();	// ��ADS1256����

	// 4.����ͨ��RLC����Ĵ����źŵ�U�ͦ�
	calculate_Amp_Phase();

	return ;
}

/**
 * @brief ��������Ƶ��
 * 
 * @param _result �����ֽ�ļ�����
 * @retval ����Ƶ���±�, ͬʱ�޸���ȫ�ֱ���
 */
int calculate_CenterFrequency()
{
	// ��Ƶ�������ֵ�㼴Ϊ����Ƶ��
	double max=math_result.amp[0];
	for(int i=0;i<NUMS;i++)
	{
		if(math_result.amp[i] >= max){
			max = math_result.amp[i];
			math_result.amp_max_index = i;
		}
	}

	// ��Ƶ������0����С�ĵ㼴Ϊ����Ƶ��
	double diff_min = fabs(math_result.phase[0] - 0.00);
	for(int i=0;i<NUMS;i++)
	{
		if(fabs(math_result.phase[i] - 0.00) <= diff_min){
			diff_min = fabs(math_result.phase[i] - 0.00);
			math_result.phase_0_index = i;
		}
	}

	// if (_result.amp_max_index == _result.phase_0_index)	printf("same amp_center vs phase_center\r\n\xff\xff\xff");
	// else printf("differnt amp_center vs phase_center\r\n amp_center:%d\r\n phase_center:%d\r\n\xff\xff\xff",_result.amp_max_index,_result.phase_0_index);
	if(curved.mode == AMP)	math_result.center_fre_index = math_result.amp_max_index;
	else	math_result.center_fre_index = math_result.phase_0_index;
	return math_result.center_fre_index;
}