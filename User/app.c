#include "app.h"
#include "adc.h"
#include "AD9854.h"
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
uint16_t adc_buf[ADCLENGTH];
volatile uint8_t adc1_dma_flag = 0;		// volatile 很重要！！！不然会卡死在while循环里
volatile uint8_t adc2_dma_flag = 0;
ADC1256 adc1256;	// 采集原始数据
RESULT math_result;	// 正交分解算法计算的结果

/*---------------------- TABLE ---------------------------*/
// 校正



/*---------------------- DRIVER ---------------------------*/
/* 选择ADC通道 */
// void changeAdcChannel(ADC_HandleTypeDef *hadc, uint32_t channel);

/** @brief 开启ADC采集-片内ADC */
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

/** @brief 开启ADC采样-ADS1256 采样率1kHz
  *	@retval 无返回值，但改变全局变量adc1256.adc1256_buf
	*/
void start_Adc_1256(void)
{
	HAL_TIM_Base_Start_IT(&htim3);
	while(!adc1256.flag);
	// for (int i = 0;i < NUMS; i++)	printf("%d\r\n", adc1256_buf[i]);
	adc1256.flag = 0;	// 清除转换完成标志位
}

/** @brief 自动扫频
  * 
  * @retval	void 
	*/
void start_FSK(void)
{
	AD9854_FSK_BPSK_HOLD_1;		// FSK = 1: 从低频向高频扫频; FSK = 0: 从高频向低频扫频
	AD9854_InitRFSK(MANUAL);									//AUTO,自动扫频，无需外加控制脚控制；MANUAL,手动扫频，由AD9854_FSK_BPSK_HOLD引脚控制扫频
	AD9854_SetRFSK(500000,600000,500,299999);	//扫频下限0.5Mhz，上限120Mhz，步进500Khz，频点间隔1ms （与ads1256采样率相对应），一共扫(120-0.5) / 0.5 = 239 个点
}

/*---------------------- FUNCTION ---------------------------*/
/**
 * @brief 正交分解算法计算待测信号的amp和phase
 * 
 * @param raw
 */
void calculate_Amp_Phase(ADC1256 raw)
{
	for(int i=0;i<NUMS;i++)
	{
		// 相频响应
		math_result.phase[i] = atan2(raw.adc1256_buf_sin[i],raw.adc1256_buf_cos[i]);
		// 幅频响应 (存在问题，Uz和Uo未知)
		math_result.amp[i] = 2.0 * sqrt(raw.adc1256_buf_cos[i]*raw.adc1256_buf_cos[i] + raw.adc1256_buf_sin[i]*raw.adc1256_buf_sin[i]);
	}
	return ;
}

/**
 * @brief 测量S21参数，正交扫频算法，只采经过乘法器、低通滤波器后的直流量
 * @param void
 * @retval void
 */
void measure_S21(void)
{
	// 1.射频开关切换为S21待测信号

	// 2.待采信号与coswt相乘
	SWITCH_RFC1_TO_RFC2;	// 切换开关, 乘法器输入为cos
	adc1256.channel = COS;
	start_FSK();			// 开启AD9854自动扫频模式
	start_Adc_1256();	// 打开ADS1256采样

	// 3.待采信号与sinwt相乘
	SWITCH_RFC1_TO_RF1;	// 切换开关, 乘法器输入为sin
	adc1256.channel = SIN;
	start_FSK();			// 开启AD9854自动扫频模式
	start_Adc_1256();	// 打开ADS1256采样

	// 4.计算通过RLC网络的待测信号的U和φ
	calculate_Amp_Phase(adc1256);

	return ;
}

/**
 * @brief 测量S11参数，正交扫频算法
 * @param void
 * @retval void
 */
void measure_S11(void)
{
	// 1.射频开关切换为S11待测信号

	// 2.待采信号与coswt相乘
	SWITCH_RFC1_TO_RFC2;	// 切换开关, 乘法器输入为cos
	adc1256.channel = COS;
	start_FSK();			// 开启AD9854自动扫频模式
	start_Adc_1256();	// 打开ADS1256采样

	// 3.待采信号与sinwt相乘
	SWITCH_RFC1_TO_RF1;	// 切换开关, 乘法器输入为sin
	adc1256.channel = SIN;
	start_FSK();			// 开启AD9854自动扫频模式
	start_Adc_1256();	// 打开ADS1256采样

	// 4.计算通过RLC网络的待测信号的U和φ
	calculate_Amp_Phase(adc1256);

	return ;
}

/*---------------------- GUI ---------------------------*/
/**
 * @brief 打印幅频、相频响应曲线
 * 
 */
void gui(void)
{
	for(int i=0;i<NUMS;i++)
	{
		// 幅频曲线
		printf("add s0.id,0,%d\xff\xff\xff",(int)math_result.amp[i]);
		// 相频曲线
		printf("add s1.id,0,%d\xff\xff\xff",(int)math_result.phase[i]);
	}
	return ;
}