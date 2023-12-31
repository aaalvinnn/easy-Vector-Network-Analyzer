/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "app.h"
#include "ADS1256.h"
#include "AD9854.h"
#include "gui.h"
#include "stdio.h"
#include "ad8302.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern volatile uint8_t ads1256_flag;
uint8_t rx_flag;		// receive from UART MENU
uint16_t ad8302_buf[10];
uint16_t mean;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
	// AD9854_InitSingle();	      	  					//AD9854点频模式初始化
	// AD9854_SetSine(12000000,4095);
  // 串口屏初始化
	// guiInit();
  // 采集序列频率数组初始化
  // for(int i=0;i<NUMS;i++) adc1256.frequency_array[i] = 5000000 + i * FRESTEP;

  // 测试，将自校正数组全填充为1
  // self_Calibration_Test();
	//receive from UART HMI
	HAL_UART_Receive_IT(&huart1, &rx_flag, 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	// start_Adc_1256();	// 打开ADS1256采样
	// for(int i=0;i<NUMS;i++)	printf("%d\r\n",adc1256.adc1256_buf_cos[i]);
	// changeAdcChannel(&hadc2, ADC_CHANNEL_1);
	// startAdc(ad8302_buf, &hadc2);
	// mean = ad8302_mean(ad8302_buf);
	// printf("增益: %.4f\r\n",ad8302_getAmp(mean));
	// printf("增益v: %.4f\r\n",(double)mean / 4095 * 3.3000);
	// changeAdcChannel(&hadc2, ADC_CHANNEL_5);
	startAdc(ad8302_buf, &hadc2);
	mean = ad8302_mean(ad8302_buf);
	printf("相位差: %.4f\r\n",ad8302_getPhase(mean));
	printf("相位差v: %.4f\r\n",(double)mean / 4095 * 3.3000);
  while (1)
  {   
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV6;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV4;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/**
 * @brief ADC DMA 转换传输完成中断回调函数
 * 
 * @param hadc 
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if(hadc == &hadc1)								//ADC1转锟斤拷锟斤拷桑锟斤拷帽锟街疚晃?1
	{
		adc1_dma_flag = 1;
		return;
	}
	else if(hadc == &hadc2)
	{
		adc2_dma_flag = 1;
		return;
	}
}
/** @brief 定时器中断回调函数
 * 
 * @param htim
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim3)
	{
		static int i = 0;
    // 分时复用ads1256的同一个通道
		if(adc1256.channel == COS)  adc1256.adc1256_buf_cos[i] = ADS1256ReadData(ADS1256_MUXP_AIN2, ADS1256_MUXN_AINCOM);
    else if(adc1256.channel == SIN)  adc1256.adc1256_buf_sin[i] = ADS1256ReadData(ADS1256_MUXP_AIN2, ADS1256_MUXN_AINCOM);
		i++;
		if(i==(NUMS))
		{
			i=0;
			HAL_TIM_Base_Stop_IT(&htim3);	// 开启ADS1256采样的方式就是打开定时器3
			adc1256.flag = 1;
		}
	}
	return ;
}

/**
 * @brief 串口接收中断回调函数,用于接收串口屏发送的指令
 * 
 * @param huart 
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)
	{
    switch (rx_flag)
    {
      // 测量S21参数
      case 0x01:
        REFRESH_CURSOR;
        REFRESH_WINDOW;
        measure_S21();
        printCurve();
        break;
      // 测量S11参数
      case 0x02:
        // REFRESH_CURSOR;
        // REFRESH_WINDOW;
        // measure_S11();
        // printCurve();
        break;
      // 幅/相频曲线显示切换
      case 0x03:
        REFRESH_CURSOR;
        REFRESH_WINDOW;
        changeCurveMode();
        printCurve();
        break;
      // error, 串口屏切换page时会输出带0x04的序列，故舍弃这个标志
      case 0x04:
        break;
      // 光标功能
      case 0x05:
        cursor.status = 1 - cursor.status;
        REFRESH_CURSOR;
        HAL_Delay(50);
        if (!cursor.status) break;    // 光标关闭
        startCursorMode();
        break;
      // 光标向右移动,步距为1
      case 0x06:
        if (!cursor.status) break;    // 光标关闭
        cursor.step = 1;
        moveCursor_Right();
        break;
      // 光标向左移动,步距为1
      case 0x07:
        if (!cursor.status) break;    // 光标关闭
        cursor.step = 1;
        moveCursor_Left();
        break;
      // 光标向右移动,步距为10
      case 0x08:
        if (!cursor.status) break;    // 光标关闭
        cursor.step = 10;
        moveCursor_Right();
        break;
      // 光标向左移动,步距为10
      case 0x09:
        if (!cursor.status) break;    // 光标关闭
        cursor.step = 10;
        moveCursor_Left();
        break;
      // 光标向右移动,步距为50
      case 0x10:
        if (!cursor.status) break;    // 光标关闭
        cursor.step = 50;
        moveCursor_Right();
        break;
      // 光标向左移动,步距为50
      case 0x11:
        if (!cursor.status) break;    // 光标关闭
        cursor.step = 50;
        moveCursor_Left();
        break;
      default:
        break;
    }
	}
	HAL_UART_Receive_IT(&huart1, &rx_flag, 1);	//receive from UART HMI
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
