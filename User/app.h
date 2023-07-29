#ifndef _USER_H
#define	_USER_H
#include "main.h"
#include "stdio.h"
#include "math.h"

#define ADCLENGTH 500 // ADCת�������С
#define NUMS 239			// ��Ƶ/��Ƶ��Ӧ���ߵ���

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
    volatile uint8_t flag;      // �ɼ�״̬��־, volatile��ֹ���������Ż����¿�����while(!flag)��
}ADC1256;

// �����ֽ��㷨����amp��phase�Ľ��
typedef struct Result{
    float amp[NUMS];
    float phase[NUMS];
}RESULT;

// ��У��ϵ��
typedef struct SelfCalibration{
    float calibration_cos[NUMS];    // �˷�������cos��У��ϵ��
    float calibration_sin[NUMS];    // �˷�������sin��У��ϵ��
}SELFCALIBRATION;

extern uint16_t adc_buf[ADCLENGTH];

void startAdc(uint16_t* _adc_buf, ADC_HandleTypeDef* hadc);
void start_Adc_1256(void);
void start_FSK(void);
void measure_S21(void);
void measure_S11(void);
void gui(void);

#endif