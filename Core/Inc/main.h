/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void user_Adc_Dma_TxCpltCallback(DMA_HandleTypeDef *hdma);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define D5_Pin GPIO_PIN_0
#define D5_GPIO_Port GPIOF
#define D6_Pin GPIO_PIN_1
#define D6_GPIO_Port GPIOF
#define A4_Pin GPIO_PIN_2
#define A4_GPIO_Port GPIOF
#define A5_Pin GPIO_PIN_3
#define A5_GPIO_Port GPIOF
#define A2_Pin GPIO_PIN_4
#define A2_GPIO_Port GPIOF
#define switch_B_Pin GPIO_PIN_0
#define switch_B_GPIO_Port GPIOC
#define switch_A_Pin GPIO_PIN_1
#define switch_A_GPIO_Port GPIOC
#define ADS1256_SCLK_Pin GPIO_PIN_3
#define ADS1256_SCLK_GPIO_Port GPIOC
#define ADS1256_MOSI_Pin GPIO_PIN_7
#define ADS1256_MOSI_GPIO_Port GPIOA
#define ADS1256_MISO_Pin GPIO_PIN_4
#define ADS1256_MISO_GPIO_Port GPIOC
#define ADS1256_DRDY_Pin GPIO_PIN_5
#define ADS1256_DRDY_GPIO_Port GPIOC
#define ADS1256_CS_Pin GPIO_PIN_2
#define ADS1256_CS_GPIO_Port GPIOB
#define RST_Pin GPIO_PIN_12
#define RST_GPIO_Port GPIOF
#define FSK_Pin GPIO_PIN_13
#define FSK_GPIO_Port GPIOF
#define A0_Pin GPIO_PIN_7
#define A0_GPIO_Port GPIOE
#define WD_Pin GPIO_PIN_8
#define WD_GPIO_Port GPIOE
#define UCLK_Pin GPIO_PIN_9
#define UCLK_GPIO_Port GPIOE
#define RD_Pin GPIO_PIN_10
#define RD_GPIO_Port GPIOE
#define OSK_Pin GPIO_PIN_12
#define OSK_GPIO_Port GPIOE
#define D0_Pin GPIO_PIN_13
#define D0_GPIO_Port GPIOE
#define vdd_Pin GPIO_PIN_14
#define vdd_GPIO_Port GPIOE
#define D2_Pin GPIO_PIN_15
#define D2_GPIO_Port GPIOE
#define D7_Pin GPIO_PIN_15
#define D7_GPIO_Port GPIOB
#define D4_Pin GPIO_PIN_8
#define D4_GPIO_Port GPIOD
#define D3_Pin GPIO_PIN_9
#define D3_GPIO_Port GPIOD
#define A3_Pin GPIO_PIN_10
#define A3_GPIO_Port GPIOD
#define D1_Pin GPIO_PIN_14
#define D1_GPIO_Port GPIOD
#define A1_Pin GPIO_PIN_15
#define A1_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
