/**
 * @file app.h
 * @author zsw (aaalvinnn@foxmail.com)
 * @brief app头文件，包含结构体定义，全局变量的声明，函数接口的声明
 * @version 0.1
 * @date 2023-07-30
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef _APP_H
#define	_APP_H

#include "stdio.h"
#include "main.h"

#define PI 3.1415926535    // 圆周率
#define ADCLENGTH 500 // ADC转换数组大小
#define NUMS 450			// 幅频/相频相应曲线点数
#define FRESTEP 100000        // 扫频步进频率
#define NOMINALCOUPLING 3.2734  // 耦合度（转换成了V单位,见对应定向耦合器的数据手册）
#define DRIFT 53511   // 乘法器的直流偏置 32mv，最后减掉 53511 = 0.032 / 0.000000598

// 待测电路与自校正电路的转换，由一对两个射频开关完成
// 开关1
#define SWITCH1_0                  HAL_GPIO_WritePin(switch1_GPIO_Port, switch1_Pin, 0)
#define SWITCH1_1                  HAL_GPIO_WritePin(switch1_GPIO_Port, switch1_Pin, 1)
// 开关2
#define SWITCH2_0                  HAL_GPIO_WritePin(switch2_GPIO_Port, switch2_Pin, 0)
#define SWITCH2_1                  HAL_GPIO_WritePin(switch2_GPIO_Port, switch2_Pin, 1)

// 开关3，控制正交分解时的输入cos/sin，分时复用一个乘法器，一个ads1256
#define SWITCH3_0                  HAL_GPIO_WritePin(switch3_GPIO_Port, switch3_Pin, 0)     // 输入cos
#define SWITCH3_1                  HAL_GPIO_WritePin(switch3_GPIO_Port, switch3_Pin, 1)     // 输入sin

// // 单刀双掷射频开关1--cos | sin
// #define SWITCH1_COS             HAL_GPIO_WritePin(switch1_A_GPIO_Port, switch1_A_Pin, 0); \
//                                 HAL_GPIO_WritePin(switch1_B_GPIO_Port, switch1_B_Pin, 1)

// #define SWITCH1_SIN             HAL_GPIO_WritePin(switch1_A_GPIO_Port, switch1_A_Pin, 1); \
//                                 HAL_GPIO_WritePin(switch1_B_GPIO_Port, switch1_B_Pin, 0)

// // 单刀双掷射频开关2--正向 | 反向
// #define SWITCH2_S21             HAL_GPIO_WritePin(switch2_A_GPIO_Port, switch2_A_Pin, 0); \
//                                 HAL_GPIO_WritePin(switch2_B_GPIO_Port, switch2_B_Pin, 1)

// #define SWITCH2_S11             HAL_GPIO_WritePin(switch2_A_GPIO_Port, switch2_A_Pin, 1); \
//                                 HAL_GPIO_WritePin(switch2_B_GPIO_Port, switch2_B_Pin, 0)
// // 单刀双掷射频开关3&4--待测RLC网络 | 自校正电路
// #define SWITCH34_RLC            HAL_GPIO_WritePin(switch3_A_GPIO_Port, switch3_A_Pin, 1); \
//                                 HAL_GPIO_WritePin(switch3_B_GPIO_Port, switch3_B_Pin, 0); \
//                                 HAL_GPIO_WritePin(switch4_A_GPIO_Port, switch4_A_Pin, 1); \
//                                 HAL_GPIO_WritePin(switch4_B_GPIO_Port, switch4_B_Pin, 0)

// #define SWITCH34_CALIBRATION    HAL_GPIO_WritePin(switch3_A_GPIO_Port, switch3_A_Pin, 0); \
//                                 HAL_GPIO_WritePin(switch3_B_GPIO_Port, switch3_B_Pin, 1); \
//                                 HAL_GPIO_WritePin(switch4_A_GPIO_Port, switch4_A_Pin, 0); \
//                                 HAL_GPIO_WritePin(switch4_B_GPIO_Port, switch4_B_Pin, 1)

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
    uint32_t frequency_array[NUMS];     // 单位为Hz
    volatile uint8_t flag;      // 采集状态标志, volatile防止被编译器优化导致卡死在while(!flag)中
}ADC1256;

// 正交分解算法计算amp和phase的结果
typedef struct Result{
    double amp[NUMS];
    double phase[NUMS];
    // 理论上amp_max_index == phase_0_index == center_fre_index
    int amp_max_index;  // 幅频响应极大值下标
    int phase_0_index;  // 相频响应零点下标
    int center_fre_index;   // 中心频率点下标
    int center_fre;     // 中心频率
}RESULT;

// 自校正系数
typedef struct SelfCalibration{
    double calibration_cos[NUMS];    // 乘法器经过cos的校正系数
    double calibration_sin[NUMS];    // 乘法器经过sin的校正系数
}SELFCALIBRATION;

extern uint16_t adc_buf[ADCLENGTH];
extern ADC1256 adc1256;	// 采集原始数据
extern RESULT math_result;	// 正交分解算法计算的结果
extern SELFCALIBRATION self_calibration;	// 自校正系数

void startAdc(uint16_t* _adc_buf, ADC_HandleTypeDef* hadc);
void start_Adc_1256(void);
void start_FSK(void);
void measure_S21(void);
void measure_S11(void);
int calculate_CenterFrequency();
void self_Calibration(void);
void self_Calibration_Test(void);

#endif