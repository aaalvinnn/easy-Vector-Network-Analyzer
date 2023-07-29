#ifndef _USER_H
#define	_USER_H
#include "main.h"
#include "stdio.h"
#include "math.h"

#define ADCLENGTH 500 // ADC转换数组大小
#define NUMS 239			// 幅频/相频相应曲线点数

// 单刀双掷射频开关1--cos | sin
#define SWITCH1_COS             HAL_GPIO_WritePin(switch1_A_GPIO_Port, switch1_A_Pin, 0); \
                                HAL_GPIO_WritePin(switch1_B_GPIO_Port, switch1_B_Pin, 1)

#define SWITCH1_SIN             HAL_GPIO_WritePin(switch1_A_GPIO_Port, switch1_A_Pin, 1); \
                                HAL_GPIO_WritePin(switch1_B_GPIO_Port, switch1_B_Pin, 0)

// 单刀双掷射频开关2--正向 | 反向
#define SWITCH2_S21             HAL_GPIO_WritePin(switch2_A_GPIO_Port, switch2_A_Pin, 0); \
                                HAL_GPIO_WritePin(switch2_B_GPIO_Port, switch2_B_Pin, 1)

#define SWITCH2_S11             HAL_GPIO_WritePin(switch2_A_GPIO_Port, switch2_A_Pin, 1); \
                                HAL_GPIO_WritePin(switch2_B_GPIO_Port, switch2_B_Pin, 0)
// 单刀双掷射频开关3&4--待测RLC网络 | 自校正电路
#define SWITCH34_RLC            HAL_GPIO_WritePin(switch3_A_GPIO_Port, switch3_A_Pin, 1); \
                                HAL_GPIO_WritePin(switch3_B_GPIO_Port, switch3_B_Pin, 0); \
                                HAL_GPIO_WritePin(switch4_A_GPIO_Port, switch4_A_Pin, 1); \
                                HAL_GPIO_WritePin(switch4_B_GPIO_Port, switch4_B_Pin, 0)

#define SWITCH34_CALIBRATION    HAL_GPIO_WritePin(switch3_A_GPIO_Port, switch3_A_Pin, 0); \
                                HAL_GPIO_WritePin(switch3_B_GPIO_Port, switch3_B_Pin, 1); \
                                HAL_GPIO_WritePin(switch4_A_GPIO_Port, switch4_A_Pin, 0); \
                                HAL_GPIO_WritePin(switch4_B_GPIO_Port, switch4_B_Pin, 1)

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
    volatile uint8_t flag;      // 采集状态标志, volatile防止被编译器优化导致卡死在while(!flag)中
}ADC1256;

// 正交分解算法计算amp和phase的结果
typedef struct Result{
    float amp[NUMS];
    float phase[NUMS];
}RESULT;

// 自校正系数
typedef struct SelfCalibration{
    float calibration_cos[NUMS];    // 乘法器经过cos的校正系数
    float calibration_sin[NUMS];    // 乘法器经过sin的校正系数
}SELFCALIBRATION;

extern uint16_t adc_buf[ADCLENGTH];

void startAdc(uint16_t* _adc_buf, ADC_HandleTypeDef* hadc);
void start_Adc_1256(void);
void start_FSK(void);
void measure_S21(void);
void measure_S11(void);
void gui(void);

#endif