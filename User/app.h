/**
 * @file app.h
 * @author zsw (aaalvinnn@foxmail.com)
 * @brief appͷ�ļ��������ṹ�嶨�壬ȫ�ֱ����������������ӿڵ�����
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
#define PI 3.1415926535    // Բ����
#define ADCLENGTH 500 // ADCת�������С
#define NUMS 400			// ��Ƶ/��Ƶ��Ӧ���ߵ���
#define FRESTEP 300000        // ɨƵ����Ƶ��

// ����˫����Ƶ����1--cos | sin
#define SWITCH1_COS             HAL_GPIO_WritePin(switch1_A_GPIO_Port, switch1_A_Pin, 0); \
                                HAL_GPIO_WritePin(switch1_B_GPIO_Port, switch1_B_Pin, 1)

#define SWITCH1_SIN             HAL_GPIO_WritePin(switch1_A_GPIO_Port, switch1_A_Pin, 1); \
                                HAL_GPIO_WritePin(switch1_B_GPIO_Port, switch1_B_Pin, 0)

// ����˫����Ƶ����2--���� | ����
#define SWITCH2_S21             HAL_GPIO_WritePin(switch2_A_GPIO_Port, switch2_A_Pin, 0); \
                                HAL_GPIO_WritePin(switch2_B_GPIO_Port, switch2_B_Pin, 1)

#define SWITCH2_S11             HAL_GPIO_WritePin(switch2_A_GPIO_Port, switch2_A_Pin, 1); \
                                HAL_GPIO_WritePin(switch2_B_GPIO_Port, switch2_B_Pin, 0)
// ����˫����Ƶ����3&4--����RLC���� | ��У����·
#define SWITCH34_RLC            HAL_GPIO_WritePin(switch3_A_GPIO_Port, switch3_A_Pin, 1); \
                                HAL_GPIO_WritePin(switch3_B_GPIO_Port, switch3_B_Pin, 0); \
                                HAL_GPIO_WritePin(switch4_A_GPIO_Port, switch4_A_Pin, 1); \
                                HAL_GPIO_WritePin(switch4_B_GPIO_Port, switch4_B_Pin, 0)

#define SWITCH34_CALIBRATION    HAL_GPIO_WritePin(switch3_A_GPIO_Port, switch3_A_Pin, 0); \
                                HAL_GPIO_WritePin(switch3_B_GPIO_Port, switch3_B_Pin, 1); \
                                HAL_GPIO_WritePin(switch4_A_GPIO_Port, switch4_A_Pin, 0); \
                                HAL_GPIO_WritePin(switch4_B_GPIO_Port, switch4_B_Pin, 1)

// �����ֽ�ʱ�ĳ���
enum Channel{
    COS,
    SIN
};

// �ɼ�����
typedef struct Adc1256{
    enum Channel channel;
    uint32_t adc1256_buf_cos[NUMS];		// �˷�������cosͨ��ɨƵ�ɼ�����ֵ
    uint32_t adc1256_buf_sin[NUMS];		// �˷�������sinͨ��ɨƵ�ɼ�����ֵ
    uint32_t frequency_array[NUMS];     // ��λΪHz
    volatile uint8_t flag;      // �ɼ�״̬��־, volatile��ֹ���������Ż����¿�����while(!flag)��
}ADC1256;

// �����ֽ��㷨����amp��phase�Ľ��
typedef struct Result{
    double amp[NUMS];
    double phase[NUMS];
    // ������amp_max_index == phase_0_index == center_fre_index
    int amp_max_index;  // ��Ƶ��Ӧ����ֵ�±�
    int phase_0_index;  // ��Ƶ��Ӧ����±�
    int center_fre_index;   // ����Ƶ�ʵ��±�
    int center_fre;     // ����Ƶ��
}RESULT;

// ��У��ϵ��
typedef struct SelfCalibration{
    double calibration_cos[NUMS];    // �˷�������cos��У��ϵ��
    double calibration_sin[NUMS];    // �˷�������sin��У��ϵ��
}SELFCALIBRATION;

extern uint16_t adc_buf[ADCLENGTH];
extern ADC1256 adc1256;	// �ɼ�ԭʼ����
extern RESULT math_result;	// �����ֽ��㷨����Ľ��
extern SELFCALIBRATION self_calibration;	// ��У��ϵ��

void startAdc(uint16_t* _adc_buf, ADC_HandleTypeDef* hadc);
void start_Adc_1256(void);
void start_FSK(void);
void measure_S21(void);
void measure_S11(void);
int calculate_CenterFrequency();
void self_Calibration(void);
void self_Calibration_Test(void);

#endif