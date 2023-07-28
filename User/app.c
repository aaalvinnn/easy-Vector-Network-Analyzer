#include "app.h"
#include "adc.h"
#include "AD9854.h"
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
uint16_t adc_buf[ADCLENGTH];
uint32_t adc1256_buf[NUMS];		// 幅频/相频相应曲线点数
volatile uint8_t adc1_dma_flag = 0;		// volatile 很重要！！！不然会卡死在while循环里
volatile uint8_t adc2_dma_flag = 0;
volatile uint8_t ads1256_flag = 0;

/*---------------------- FUNCTION ---------------------------*/
/* 选择ADC通道 */
// void changeAdcChannel(ADC_HandleTypeDef *hadc, uint32_t channel);

/** @brif 开启ADC采集-片内ADC */
void startAdc(uint16_t* _adc_buf, ADC_HandleTypeDef* hadc)
{
  if(hadc == &hadc1)
  {
    HAL_TIM_Base_Start_IT(&htim2);
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)_adc_buf, ADCLENGTH); 
		while(!adc1_dma_flag);    // 等待DMA传输完成
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

/** @brif 开启ADC采样-ADS1256 采样率1kHz
  *	@retval 无返回值，但改变全局变量adc1256_buf
	*/
void start_Adc_1256(void)
{
	HAL_TIM_Base_Start_IT(&htim3);
	while(!ads1256_flag);
	// for (int i = 0;i < NUMS; i++)	printf("%d\r\n", adc1256_buf[i]);
	ads1256_flag = 0;	// 清除转换完成标志位
}

/** @brif 自动扫频
	*/
void start_FSK(void)
{
	AD9854_FSK_BPSK_HOLD_1;
	AD9854_InitRFSK(MANUAL);									//AUTO,自动扫频，无需外加控制脚控制；MANUAL,手动扫频，由AD9854_FSK_BPSK_HOLD引脚控制扫频
	AD9854_SetRFSK(1000,600000,500,1048575);	//扫频下限1Khz，上限60Khz，步进100hz，扫频时间约2S，扫频时间计算参考函数注解
}