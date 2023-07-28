#ifndef _USER_H
#define	_USER_H
#include "main.h"
#include "stdio.h"

#define ADCLENGTH 500 // ADC转换数组大小
#define NUMS 239			// 幅频/相频相应曲线点数

// 单刀双掷射频开关1
#define SWITCH_RFC1_TO_RFC2 HAL_GPIO_WritePin(switch_A_GPIO_Port, switch_A_Pin, 0); \
                            HAL_GPIO_WritePin(switch_B_GPIO_Port, switch_B_Pin, 1)
// 单刀双掷射频开关2
#define SWITCH_RFC1_TO_RF1  HAL_GPIO_WritePin(switch_A_GPIO_Port, switch_A_Pin, 1); \
                            HAL_GPIO_WritePin(switch_B_GPIO_Port, switch_B_Pin, 0)
                            
// 正交分解时的乘数
enum Channel{
    COS,
    SIN
};

// 采集对象
typedef struct Adc1256{
    enum Channel channel;
    uint32_t adc1256_buf_cos[NUMS];		// 乘法器经过cos通道扫频采集到的值
    uint32_t adc1256_buf_sin[NUMS];		// 乘法器经过sin通道扫频采集到的值
    volatile uint8_t flag;      // 采集状态标志
}ADC1256;

extern uint16_t adc_buf[ADCLENGTH];

void startAdc(uint16_t* _adc_buf, ADC_HandleTypeDef* hadc);
void start_Adc_1256(void);
void start_FSK(void);

#endif