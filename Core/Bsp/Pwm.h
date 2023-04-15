#ifndef __PWM_H
#define __PWM_H

#include "main.h"
#include "cmsis_os.h"
#include "stm32f1xx_hal.h"

/**
 * VirPwm_SetOutput是函数指针类型变量，用来回调端口输出函数
 */
typedef void (*VirPwm_SetOutput)(uint8_t state);

/**
 * 虚拟PWM的结构体类型，通过此变量类型可以完整的定义一个虚拟PWM，以供本库的函数操作
 */
typedef struct{
	uint8_t Status;
	uint8_t Idle;
	uint16_t Frequency;
	uint16_t DutyCycle;
	TIM_TypeDef *Basetimer;
	VirPwm_SetOutput SetOut;
    uint8_t count_P;
    uint8_t count_N;
} VirPwm;

/**
 * 与平台相关， 操作GPIO的宏定义
 */
// #define DIG_OUTPUT_1(status)			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_13, status)

extern VirPwm VirPwmDef;

void VirPwm_Init(VirPwm *VirPwmDef,uint16_t freq,uint16_t dutyCycle,uint8_t idle, VirPwm_SetOutput setOutHandler);
void VirPwm_SetStatus(VirPwm *VirPwmDef, uint8_t status);
void VirPwm_SetFreq(VirPwm *VirPwmDef, uint16_t freq);
void VirPwm_SetDutyCycle(VirPwm *VirPwmDef, uint16_t dutyCycle);
void VirPwm_SetIdle(VirPwm *VirPwmDef, uint8_t idle);
void VirPwm_TimIRQHandler(VirPwm *VirPwmDef);
void VirPwm_SetOutHandler(uint8_t state);

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */


#endif /* __LED_H */