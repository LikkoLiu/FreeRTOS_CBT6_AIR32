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
extern VirPwm VirPwmDef1;
extern VirPwm VirPwmDef2;
extern VirPwm VirPwmDef3;
extern VirPwm VirPwmDef4;
extern VirPwm VirPwmDef5;
extern VirPwm VirPwmDef6;
extern VirPwm VirPwmDef7;
extern VirPwm VirPwmDef8;
extern VirPwm VirPwmDef9;
extern VirPwm VirPwmDef10;
extern VirPwm VirPwmDef11;
extern VirPwm VirPwmDef12;
extern VirPwm VirPwmDef13;
extern VirPwm VirPwmDef14;
extern VirPwm VirPwmDef15;
extern VirPwm VirPwmDef16;
extern VirPwm VirPwmDef17;
extern VirPwm VirPwmDef18;
extern VirPwm VirPwmDef19;
extern VirPwm VirPwmDef20;
extern VirPwm VirPwmDef21;
extern VirPwm VirPwmDef22;
extern VirPwm VirPwmDef23;
extern VirPwm VirPwmDef24;
extern VirPwm VirPwmDef25;
extern VirPwm VirPwmDef26;
extern VirPwm VirPwmDef27;
extern VirPwm VirPwmDef28;
extern VirPwm VirPwmDef29;
extern VirPwm VirPwmDef30;
extern VirPwm VirPwmDef31;


void VirPwm_SetOutHandler1(uint8_t state);
void VirPwm_SetOutHandler2(uint8_t state);
void VirPwm_SetOutHandler3(uint8_t state);
void VirPwm_SetOutHandler4(uint8_t state);
void VirPwm_SetOutHandler5(uint8_t state);
void VirPwm_SetOutHandler6(uint8_t state);
void VirPwm_SetOutHandler7(uint8_t state);
void VirPwm_SetOutHandler8(uint8_t state);
void VirPwm_SetOutHandler9(uint8_t state);
void VirPwm_SetOutHandler10(uint8_t state);
void VirPwm_SetOutHandler11(uint8_t state);
void VirPwm_SetOutHandler12(uint8_t state);
void VirPwm_SetOutHandler13(uint8_t state);
void VirPwm_SetOutHandler14(uint8_t state);
void VirPwm_SetOutHandler15(uint8_t state);
void VirPwm_SetOutHandler16(uint8_t state);
void VirPwm_SetOutHandler17(uint8_t state);
void VirPwm_SetOutHandler18(uint8_t state);
void VirPwm_SetOutHandler19(uint8_t state);
void VirPwm_SetOutHandler20(uint8_t state);
void VirPwm_SetOutHandler21(uint8_t state);
void VirPwm_SetOutHandler22(uint8_t state);
void VirPwm_SetOutHandler23(uint8_t state);
void VirPwm_SetOutHandler24(uint8_t state);
void VirPwm_SetOutHandler25(uint8_t state);
void VirPwm_SetOutHandler26(uint8_t state);
void VirPwm_SetOutHandler27(uint8_t state);
void VirPwm_SetOutHandler28(uint8_t state);
void VirPwm_SetOutHandler29(uint8_t state);
void VirPwm_SetOutHandler30(uint8_t state);
void VirPwm_SetOutHandler31(uint8_t state);
/* USER CODE END Application */


#endif /* __LED_H */