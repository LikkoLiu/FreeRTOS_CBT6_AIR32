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
#include "cmsis_os.h"
#include "tim.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
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
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  Modbus_RegMap();
  HAL_Delay(2000);

  // VirPwm_Init(&VirPwmDef1, VirPwmDef1.Frequency, VirPwmDef1.DutyCycle, VirPwmDef1.Idle, VirPwmDef1.SetOut);
  VirPwm_Init(&VirPwmDef2, VirPwmDef2.Frequency, VirPwmDef2.DutyCycle, VirPwmDef2.Idle, VirPwmDef2.SetOut);
  VirPwm_Init(&VirPwmDef3, VirPwmDef3.Frequency, VirPwmDef3.DutyCycle, VirPwmDef3.Idle, VirPwmDef3.SetOut);
  VirPwm_Init(&VirPwmDef4, VirPwmDef4.Frequency, VirPwmDef4.DutyCycle, VirPwmDef4.Idle, VirPwmDef4.SetOut);
  VirPwm_Init(&VirPwmDef5, VirPwmDef5.Frequency, VirPwmDef5.DutyCycle, VirPwmDef5.Idle, VirPwmDef5.SetOut);
  VirPwm_Init(&VirPwmDef5, VirPwmDef5.Frequency, VirPwmDef5.DutyCycle, VirPwmDef5.Idle, VirPwmDef5.SetOut);
  VirPwm_Init(&VirPwmDef6, VirPwmDef6.Frequency, VirPwmDef6.DutyCycle, VirPwmDef6.Idle, VirPwmDef6.SetOut);
  VirPwm_Init(&VirPwmDef7, VirPwmDef7.Frequency, VirPwmDef7.DutyCycle, VirPwmDef7.Idle, VirPwmDef7.SetOut);
  VirPwm_Init(&VirPwmDef8, VirPwmDef8.Frequency, VirPwmDef8.DutyCycle, VirPwmDef8.Idle, VirPwmDef8.SetOut);
  VirPwm_Init(&VirPwmDef9, VirPwmDef9.Frequency, VirPwmDef9.DutyCycle, VirPwmDef9.Idle, VirPwmDef9.SetOut);
  VirPwm_Init(&VirPwmDef10, VirPwmDef10.Frequency, VirPwmDef10.DutyCycle, VirPwmDef10.Idle, VirPwmDef10.SetOut);
  VirPwm_Init(&VirPwmDef11, VirPwmDef11.Frequency, VirPwmDef11.DutyCycle, VirPwmDef11.Idle, VirPwmDef11.SetOut);
  VirPwm_Init(&VirPwmDef12, VirPwmDef12.Frequency, VirPwmDef12.DutyCycle, VirPwmDef12.Idle, VirPwmDef12.SetOut);
  VirPwm_Init(&VirPwmDef13, VirPwmDef13.Frequency, VirPwmDef13.DutyCycle, VirPwmDef13.Idle, VirPwmDef13.SetOut);
  VirPwm_Init(&VirPwmDef14, VirPwmDef14.Frequency, VirPwmDef14.DutyCycle, VirPwmDef14.Idle, VirPwmDef14.SetOut);
  VirPwm_Init(&VirPwmDef15, VirPwmDef15.Frequency, VirPwmDef15.DutyCycle, VirPwmDef15.Idle, VirPwmDef15.SetOut);
  VirPwm_Init(&VirPwmDef16, VirPwmDef16.Frequency, VirPwmDef16.DutyCycle, VirPwmDef16.Idle, VirPwmDef16.SetOut);
  VirPwm_Init(&VirPwmDef17, VirPwmDef17.Frequency, VirPwmDef17.DutyCycle, VirPwmDef17.Idle, VirPwmDef17.SetOut);
  VirPwm_Init(&VirPwmDef18, VirPwmDef18.Frequency, VirPwmDef18.DutyCycle, VirPwmDef18.Idle, VirPwmDef18.SetOut);
  VirPwm_Init(&VirPwmDef19, VirPwmDef19.Frequency, VirPwmDef19.DutyCycle, VirPwmDef19.Idle, VirPwmDef19.SetOut);
  VirPwm_Init(&VirPwmDef20, VirPwmDef20.Frequency, VirPwmDef20.DutyCycle, VirPwmDef20.Idle, VirPwmDef20.SetOut);
  VirPwm_Init(&VirPwmDef21, VirPwmDef21.Frequency, VirPwmDef21.DutyCycle, VirPwmDef21.Idle, VirPwmDef21.SetOut);
  VirPwm_Init(&VirPwmDef22, VirPwmDef22.Frequency, VirPwmDef22.DutyCycle, VirPwmDef22.Idle, VirPwmDef22.SetOut);
  VirPwm_Init(&VirPwmDef23, VirPwmDef23.Frequency, VirPwmDef23.DutyCycle, VirPwmDef23.Idle, VirPwmDef23.SetOut);
  VirPwm_Init(&VirPwmDef24, VirPwmDef24.Frequency, VirPwmDef24.DutyCycle, VirPwmDef24.Idle, VirPwmDef24.SetOut);
  VirPwm_Init(&VirPwmDef25, VirPwmDef25.Frequency, VirPwmDef25.DutyCycle, VirPwmDef25.Idle, VirPwmDef25.SetOut);
  VirPwm_Init(&VirPwmDef26, VirPwmDef26.Frequency, VirPwmDef26.DutyCycle, VirPwmDef26.Idle, VirPwmDef26.SetOut);
  VirPwm_Init(&VirPwmDef27, VirPwmDef27.Frequency, VirPwmDef27.DutyCycle, VirPwmDef27.Idle, VirPwmDef27.SetOut);
  VirPwm_Init(&VirPwmDef28, VirPwmDef28.Frequency, VirPwmDef28.DutyCycle, VirPwmDef28.Idle, VirPwmDef28.SetOut);
  VirPwm_Init(&VirPwmDef29, VirPwmDef29.Frequency, VirPwmDef29.DutyCycle, VirPwmDef29.Idle, VirPwmDef29.SetOut);
  VirPwm_Init(&VirPwmDef30, VirPwmDef30.Frequency, VirPwmDef30.DutyCycle, VirPwmDef30.Idle, VirPwmDef30.SetOut);
  VirPwm_Init(&VirPwmDef31, VirPwmDef31.Frequency, VirPwmDef31.DutyCycle, VirPwmDef31.Idle, VirPwmDef31.SetOut);

  /* USER CODE END 2 */

  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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

#ifdef USE_FULL_ASSERT
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
