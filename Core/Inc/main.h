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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Modbus.h"
#include "Pwm.h"
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

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define PWM1_Pin GPIO_PIN_13
#define PWM1_GPIO_Port GPIOC
#define PWM2_Pin GPIO_PIN_14
#define PWM2_GPIO_Port GPIOC
#define PWM3_Pin GPIO_PIN_15
#define PWM3_GPIO_Port GPIOC
#define PWM4_Pin GPIO_PIN_0
#define PWM4_GPIO_Port GPIOA
#define PWM5_Pin GPIO_PIN_1
#define PWM5_GPIO_Port GPIOA
#define PWM6_Pin GPIO_PIN_2
#define PWM6_GPIO_Port GPIOA
#define PWM7_Pin GPIO_PIN_3
#define PWM7_GPIO_Port GPIOA
#define PWM8_Pin GPIO_PIN_4
#define PWM8_GPIO_Port GPIOA
#define PWM9_Pin GPIO_PIN_5
#define PWM9_GPIO_Port GPIOA
#define PWM10_Pin GPIO_PIN_6
#define PWM10_GPIO_Port GPIOA
#define PWM11_Pin GPIO_PIN_7
#define PWM11_GPIO_Port GPIOA
#define PWM16_Pin GPIO_PIN_0
#define PWM16_GPIO_Port GPIOB
#define PWM17_Pin GPIO_PIN_1
#define PWM17_GPIO_Port GPIOB
#define PWM18_Pin GPIO_PIN_2
#define PWM18_GPIO_Port GPIOB
#define PWM26_Pin GPIO_PIN_10
#define PWM26_GPIO_Port GPIOB
#define PWM27_Pin GPIO_PIN_11
#define PWM27_GPIO_Port GPIOB
#define PWM28_Pin GPIO_PIN_12
#define PWM28_GPIO_Port GPIOB
#define PWM29_Pin GPIO_PIN_13
#define PWM29_GPIO_Port GPIOB
#define PWM30_Pin GPIO_PIN_14
#define PWM30_GPIO_Port GPIOB
#define PWM31_Pin GPIO_PIN_15
#define PWM31_GPIO_Port GPIOB
#define PWM12_Pin GPIO_PIN_8
#define PWM12_GPIO_Port GPIOA
#define PWM13_Pin GPIO_PIN_9
#define PWM13_GPIO_Port GPIOA
#define PWM14_Pin GPIO_PIN_10
#define PWM14_GPIO_Port GPIOA
#define PWM15_Pin GPIO_PIN_15
#define PWM15_GPIO_Port GPIOA
#define PWM19_Pin GPIO_PIN_3
#define PWM19_GPIO_Port GPIOB
#define PWM20_Pin GPIO_PIN_4
#define PWM20_GPIO_Port GPIOB
#define PWM21_Pin GPIO_PIN_5
#define PWM21_GPIO_Port GPIOB
#define PWM22_Pin GPIO_PIN_6
#define PWM22_GPIO_Port GPIOB
#define PWM23_Pin GPIO_PIN_7
#define PWM23_GPIO_Port GPIOB
#define PWM24_Pin GPIO_PIN_8
#define PWM24_GPIO_Port GPIOB
#define PWM25_Pin GPIO_PIN_9
#define PWM25_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
