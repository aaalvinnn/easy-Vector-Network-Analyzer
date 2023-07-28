#ifndef _USER_H
#define	_USER_H
#include "main.h"
#include "stdio.h"

#define ADCLENGTH 500 // ADCת�������С
#define NUMS 239			// ��Ƶ/��Ƶ��Ӧ���ߵ���

// ����˫����Ƶ����1
#define SWITCH_RFC1_TO_RFC2 HAL_GPIO_WritePin(switch_A_GPIO_Port, switch_A_Pin, 0); \
                            HAL_GPIO_WritePin(switch_B_GPIO_Port, switch_B_Pin, 1)
// ����˫����Ƶ����2
#define SWITCH_RFC1_TO_RF1  HAL_GPIO_WritePin(switch_A_GPIO_Port, switch_A_Pin, 1); \
                            HAL_GPIO_WritePin(switch_B_GPIO_Port, switch_B_Pin, 0)
                            
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
    volatile uint8_t flag;      // �ɼ�״̬��־
}ADC1256;

extern uint16_t adc_buf[ADCLENGTH];

void startAdc(uint16_t* _adc_buf, ADC_HandleTypeDef* hadc);
void start_Adc_1256(void);
void start_FSK(void);

#endif