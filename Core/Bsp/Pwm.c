#include "Pwm.h"

/**
 * 使用虚拟PWM库时，应当在他处配置好定时器（由宏定义VIRPWM_TIMER指定），
 * 并在该定时器的中断服务函数中调用void VirPwm_TimIRQHandler()。
 * 同时，应该在外部为void (*VirPwm_SetOutputHandler)(uint8_t state)注册一个函数，
 * 以实现端口的电平翻转操作。
 */

VirPwm VirPwmDef = {
    .Status = 0,
    .Idle = 0,
    .Frequency = 100,
    .DutyCycle = 50,
    .Basetimer = TIM4,
    .SetOut = VirPwm_SetOutHandler,
    .count_P = 0,
    .count_N = 100};

/**
 * @brief 完成初始化VirPwmDef结构体指针
 * 		也可以直接在定义结构体的时候完成初始化，而不必调用本函数
 * @param VirPwm: 虚拟PWM的定义结构体指针
 * @param freq: 虚拟PWM的频率
 * @param dutyCycle: 虚拟PWM的占空比
 * @param idle: 虚拟PWM的空闲状态
 */
void VirPwm_Init(VirPwm *VirPwmDef, uint16_t freq, uint16_t dutyCycle, uint8_t idle, VirPwm_SetOutput setOutHandler)
{
    VirPwm_SetFreq(VirPwmDef, freq);
    VirPwm_SetDutyCycle(VirPwmDef, dutyCycle);
    VirPwm_SetIdle(VirPwmDef, idle);
}

/**
 * @brief 设置虚拟PWM的工作状态
 * @param VirPwm: 需要操作的虚拟PWM
 * @param status: 0, disable; 1, enable
 */
// void VirPwm_SetStatus(VirPwm *VirPwmDef, uint8_t status)
// {
// 	if(status != 0)
// 	{
// 		VirPwmDef->Status = 1;
// 	}
// 	else
// 	{
// 		VirPwmDef->Status = 0;
// 	}

// 	//设置驱动虚拟PWM的定时器更新中断周期
// 	VirPwm_SetFreq(VirPwmDef, VirPwmDef->Frequency);
// 	//开启或关闭驱动虚拟PWM的定时器
// 	TIM_Cmd(VirPwmDef->Basetimer, VirPwmDef->Status);

// 	//关闭虚拟PWM之后，将输出电平恢复为idle状态
// 	if(VirPwmDef->Status == 0)
// 	{
// 		if(VirPwmDef->Idle != 0)
// 		{
// 			VirPwmDef->SetOut(1);
// 		}
// 		else
// 		{
// 			VirPwmDef->SetOut(0);
// 		}
// 	}
// }

/**
 * @brief 设置虚拟PWM的频率
 * 		虚拟PWM的频率*100即为定时器的更新频
 * @param VirPwm: 需要操作的虚拟PWM
 * @param freq: frequency of the virtual pwm, ranged from 1~1000 Hz
 * 		由于虚拟PWM拥有较低的效率，占用较多的CPU时间，因此不能设置较高的工作频率，否则会影响系统的正常工作。
 */
void VirPwm_SetFreq(VirPwm *VirPwmDef, uint16_t freq)
{
    // 0~100 duty cycle
    uint32_t timer_freq;

    // Range of frequency is 1 to 1000.
    if (freq > 1000)
    {
        freq = 1000;
    }

    if (freq < 1)
    {
        freq = 1;
    }

    VirPwmDef->Frequency = freq;

    timer_freq = VirPwmDef->Frequency * 100; // 100~100k

    // timer_freq < 1k, psc = 7200, f = 10kHz 100*100 Hz (freq < 10)
    if (timer_freq < 1000)
    {
        VirPwmDef->Basetimer->PSC = 7200 - 1;
    }
    // timer_freq < 10k, psc = 720, f = 100kHz 1k*100 Hz (freq < 100)
    else if (timer_freq < 10000)
    {
        VirPwmDef->Basetimer->PSC = 720 - 1;
    }
    // timer_freq < 100k, psc = 72, f = 1MHz 10k*100 Hz (freq < 1000)
    else
    {
        VirPwmDef->Basetimer->PSC = 72 - 1;
    }

    // Set the update frequency of the timer.
    VirPwmDef->Basetimer->ARR = 72000000.0 / (VirPwmDef->Basetimer->PSC + 1) / timer_freq - 1;
}

/**
 * @brief 设置虚拟PWM的占空比
 * @param VirPwm: 需要操作的虚拟PWM
 * @param dutyCycle: duty cycle of the virtual pwm, ranged from 1~100 (%).
 * 		由于性能限制，虚拟PWM支持的占空比分辨率为1%，范围为0~100%。
 */
void VirPwm_SetDutyCycle(VirPwm *VirPwmDef, uint16_t dutyCycle)
{
    // Range of duty cycle is 0 to 100.
    if (dutyCycle > 100)
    {
        dutyCycle = 100;
    }

    VirPwmDef->DutyCycle = dutyCycle;
}

/**
 * @brief 设置虚拟PWM的极性
 * @param VirPwm: 需要操作的虚拟PWM
 * @param idle:	虚拟PWM在空闲状态下的电平
 */
void VirPwm_SetIdle(VirPwm *VirPwmDef, uint8_t idle)
{
    if (idle != 0)
    {
        VirPwmDef->Idle = 1;
    }
    else
    {
        VirPwmDef->Idle = 0;
    }
}

/**
 * @brief 在定时器中断中周期性调用
 * @param VirPwm: 需要操作的虚拟PWM
 */
void VirPwm_TimIRQHandler(VirPwm *VirPwmDef)
{
    // 占空比为0，则直接关闭PWM输出，将端口设置为idle状态
    if (VirPwmDef->DutyCycle == 0)
    {
        if (VirPwmDef->Idle != 0)
        {
            VirPwmDef->SetOut(1);
        }
        else
        {
            VirPwmDef->SetOut(0);
        }

        return;
    }

    // 占空比为100，则直接打开PWM输出，将端口设置为busy状态
    if (VirPwmDef->DutyCycle >= 100)
    {
        if (VirPwmDef->Idle != 0)
        {
            VirPwmDef->SetOut(0);
        }
        else
        {
            VirPwmDef->SetOut(1);
        }

        return;
    }

    // 占空比为1~99时，正常处理
    if (VirPwmDef->count_P == 0)
    {
        if (VirPwmDef->count_N == 0)
        {
            VirPwmDef->count_P = VirPwmDef->DutyCycle;
            VirPwmDef->count_N = 100 - VirPwmDef->DutyCycle;
            VirPwmDef->SetOut(1);
        }
        else
        {
            VirPwmDef->count_N--;
            VirPwmDef->SetOut(0);
        }
    }
    else
    {
        VirPwmDef->count_P--;
        VirPwmDef->SetOut(1);
    }
}

/**
 * @brief 注册为实现端口输出SetOut的服务函数（与具体项目相关）
 * @param VirPwm: 需要操作的虚拟PWM
 */
void VirPwm_SetOutHandler(uint8_t state)
{
    // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_13, state);
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
VirPwm VirPwmDef1 = {
    .Status = 0,
    .Idle = 0,
    .Frequency = 150,
    .DutyCycle = 75,
    .Basetimer = TIM1,
    .SetOut = VirPwm_SetOutHandler1,
    .count_P = 0,
    .count_N = 100};
VirPwm VirPwmDef2 = {
    .Status = 0,
    .Idle = 0,
    .Frequency = 150,
    .DutyCycle = 75,
    .Basetimer = TIM1,
    .SetOut = VirPwm_SetOutHandler2,
    .count_P = 0,
    .count_N = 100};
VirPwm VirPwmDef3 = {
    .Status = 0,
    .Idle = 0,
    .Frequency = 150,
    .DutyCycle = 75,
    .Basetimer = TIM1,
    .SetOut = VirPwm_SetOutHandler3,
    .count_P = 0,
    .count_N = 100};
VirPwm VirPwmDef4 = {
    .Status = 0,
    .Idle = 0,
    .Frequency = 150,
    .DutyCycle = 75,
    .Basetimer = TIM1,
    .SetOut = VirPwm_SetOutHandler4,
    .count_P = 0,
    .count_N = 100};
VirPwm VirPwmDef5 = {
    .Status = 0,
    .Idle = 0,
    .Frequency = 150,
    .DutyCycle = 75,
    .Basetimer = TIM1,
    .SetOut = VirPwm_SetOutHandler5,
    .count_P = 0,
    .count_N = 100};
VirPwm VirPwmDef6 = {
    .Status = 0,
    .Idle = 0,
    .Frequency = 150,
    .DutyCycle = 75,
    .Basetimer = TIM1,
    .SetOut = VirPwm_SetOutHandler6,
    .count_P = 0,
    .count_N = 100};
VirPwm VirPwmDef7 = {
    .Status = 0,
    .Idle = 0,
    .Frequency = 150,
    .DutyCycle = 75,
    .Basetimer = TIM1,
    .SetOut = VirPwm_SetOutHandler7,
    .count_P = 0,
    .count_N = 100};
VirPwm VirPwmDef8 = {
    .Status = 0,
    .Idle = 0,
    .Frequency = 150,
    .DutyCycle = 75,
    .Basetimer = TIM1,
    .SetOut = VirPwm_SetOutHandler8,
    .count_P = 0,
    .count_N = 100};
VirPwm VirPwmDef9 = {
    .Status = 0,
    .Idle = 0,
    .Frequency = 150,
    .DutyCycle = 75,
    .Basetimer = TIM1,
    .SetOut = VirPwm_SetOutHandler9,
    .count_P = 0,
    .count_N = 100};
VirPwm VirPwmDef10 = {
    .Status = 0,
    .Idle = 0,
    .Frequency = 150,
    .DutyCycle = 75,
    .Basetimer = TIM1,
    .SetOut = VirPwm_SetOutHandler10,
    .count_P = 0,
    .count_N = 100};
VirPwm VirPwmDef11 = {
    .Status = 0,
    .Idle = 0,
    .Frequency = 150,
    .DutyCycle = 75,
    .Basetimer = TIM2,
    .SetOut = VirPwm_SetOutHandler11,
    .count_P = 0,
    .count_N = 100};
VirPwm VirPwmDef12 = {
    .Status = 0,
    .Idle = 0,
    .Frequency = 150,
    .DutyCycle = 75,
    .Basetimer = TIM2,
    .SetOut = VirPwm_SetOutHandler12,
    .count_P = 0,
    .count_N = 100};
VirPwm VirPwmDef13 = {
    .Status = 0,
    .Idle = 0,
    .Frequency = 150,
    .DutyCycle = 75,
    .Basetimer = TIM2,
    .SetOut = VirPwm_SetOutHandler13,
    .count_P = 0,
    .count_N = 100};
VirPwm VirPwmDef14 = {
    .Status = 0,
    .Idle = 0,
    .Frequency = 150,
    .DutyCycle = 75,
    .Basetimer = TIM2,
    .SetOut = VirPwm_SetOutHandler14,
    .count_P = 0,
    .count_N = 100};
VirPwm VirPwmDef15 = {
    .Status = 0,
    .Idle = 0,
    .Frequency = 150,
    .DutyCycle = 75,
    .Basetimer = TIM2,
    .SetOut = VirPwm_SetOutHandler15,
    .count_P = 0,
    .count_N = 100};
VirPwm VirPwmDef16 = {
    .Status = 0,
    .Idle = 0,
    .Frequency = 150,
    .DutyCycle = 75,
    .Basetimer = TIM2,
    .SetOut = VirPwm_SetOutHandler16,
    .count_P = 0,
    .count_N = 100};
VirPwm VirPwmDef17 = {
    .Status = 0,
    .Idle = 0,
    .Frequency = 150,
    .DutyCycle = 75,
    .Basetimer = TIM2,
    .SetOut = VirPwm_SetOutHandler17,
    .count_P = 0,
    .count_N = 100};
VirPwm VirPwmDef18 = {
    .Status = 0,
    .Idle = 0,
    .Frequency = 150,
    .DutyCycle = 75,
    .Basetimer = TIM2,
    .SetOut = VirPwm_SetOutHandler18,
    .count_P = 0,
    .count_N = 100};
VirPwm VirPwmDef19 = {
    .Status = 0,
    .Idle = 0,
    .Frequency = 150,
    .DutyCycle = 75,
    .Basetimer = TIM2,
    .SetOut = VirPwm_SetOutHandler19,
    .count_P = 0,
    .count_N = 100};
VirPwm VirPwmDef20 = {
    .Status = 0,
    .Idle = 0,
    .Frequency = 150,
    .DutyCycle = 75,
    .Basetimer = TIM2,
    .SetOut = VirPwm_SetOutHandler20,
    .count_P = 0,
    .count_N = 100};
VirPwm VirPwmDef21 = {
    .Status = 0,
    .Idle = 0,
    .Frequency = 150,
    .DutyCycle = 75,
    .Basetimer = TIM3,
    .SetOut = VirPwm_SetOutHandler21,
    .count_P = 0,
    .count_N = 100};
VirPwm VirPwmDef22 = {
    .Status = 0,
    .Idle = 0,
    .Frequency = 150,
    .DutyCycle = 75,
    .Basetimer = TIM3,
    .SetOut = VirPwm_SetOutHandler22,
    .count_P = 0,
    .count_N = 100};
VirPwm VirPwmDef23 = {
    .Status = 0,
    .Idle = 0,
    .Frequency = 150,
    .DutyCycle = 75,
    .Basetimer = TIM3,
    .SetOut = VirPwm_SetOutHandler23,
    .count_P = 0,
    .count_N = 100};
VirPwm VirPwmDef24 = {
    .Status = 0,
    .Idle = 0,
    .Frequency = 150,
    .DutyCycle = 75,
    .Basetimer = TIM3,
    .SetOut = VirPwm_SetOutHandler24,
    .count_P = 0,
    .count_N = 100};
VirPwm VirPwmDef25 = {
    .Status = 0,
    .Idle = 0,
    .Frequency = 150,
    .DutyCycle = 75,
    .Basetimer = TIM3,
    .SetOut = VirPwm_SetOutHandler25,
    .count_P = 0,
    .count_N = 100};
VirPwm VirPwmDef26 = {
    .Status = 0,
    .Idle = 0,
    .Frequency = 150,
    .DutyCycle = 75,
    .Basetimer = TIM3,
    .SetOut = VirPwm_SetOutHandler26,
    .count_P = 0,
    .count_N = 100};
VirPwm VirPwmDef27 = {
    .Status = 0,
    .Idle = 0,
    .Frequency = 150,
    .DutyCycle = 75,
    .Basetimer = TIM3,
    .SetOut = VirPwm_SetOutHandler27,
    .count_P = 0,
    .count_N = 100};
VirPwm VirPwmDef28 = {
    .Status = 0,
    .Idle = 0,
    .Frequency = 150,
    .DutyCycle = 75,
    .Basetimer = TIM3,
    .SetOut = VirPwm_SetOutHandler28,
    .count_P = 0,
    .count_N = 100};
VirPwm VirPwmDef29 = {
    .Status = 0,
    .Idle = 0,
    .Frequency = 150,
    .DutyCycle = 75,
    .Basetimer = TIM3,
    .SetOut = VirPwm_SetOutHandler29,
    .count_P = 0,
    .count_N = 100};
VirPwm VirPwmDef30 = {
    .Status = 0,
    .Idle = 0,
    .Frequency = 150,
    .DutyCycle = 75,
    .Basetimer = TIM3,
    .SetOut = VirPwm_SetOutHandler30,
    .count_P = 0,
    .count_N = 100};
VirPwm VirPwmDef31 = { 
    .Status = 0,
    .Idle = 0,
    .Frequency = 150,
    .DutyCycle = 75,
    .Basetimer = TIM1,
    .SetOut = VirPwm_SetOutHandler31,
    .count_P = 0,
    .count_N = 100};

/**
 * @brief 注册为实现端口输出SetOut的服务函数（输出PWM）
 * @param VirPwm: 需要操作的虚拟PWM
 */
void VirPwm_SetOutHandler1(uint8_t state)
{
    HAL_GPIO_WritePin(PWM1_GPIO_Port, PWM1_Pin, state);
}
void VirPwm_SetOutHandler2(uint8_t state)
{
    HAL_GPIO_WritePin(PWM2_GPIO_Port, PWM2_Pin, state);
}
void VirPwm_SetOutHandler3(uint8_t state)
{
    HAL_GPIO_WritePin(PWM3_GPIO_Port, PWM3_Pin, state);
}
void VirPwm_SetOutHandler4(uint8_t state)
{
    HAL_GPIO_WritePin(PWM4_GPIO_Port, PWM4_Pin, state);
}
void VirPwm_SetOutHandler5(uint8_t state)
{
    HAL_GPIO_WritePin(PWM5_GPIO_Port, PWM5_Pin, state);
}  
void VirPwm_SetOutHandler6(uint8_t state)
{
    HAL_GPIO_WritePin(PWM6_GPIO_Port, PWM6_Pin, state);
}
void VirPwm_SetOutHandler7(uint8_t state)
{
    HAL_GPIO_WritePin(PWM7_GPIO_Port, PWM7_Pin, state);
}
void VirPwm_SetOutHandler8(uint8_t state)
{
    HAL_GPIO_WritePin(PWM8_GPIO_Port, PWM8_Pin, state);
}
void VirPwm_SetOutHandler9(uint8_t state)
{
    HAL_GPIO_WritePin(PWM9_GPIO_Port, PWM9_Pin, state);
}
void VirPwm_SetOutHandler10(uint8_t state)
{
    HAL_GPIO_WritePin(PWM10_GPIO_Port, PWM10_Pin, state);
}
void VirPwm_SetOutHandler11(uint8_t state)
{
    HAL_GPIO_WritePin(PWM11_GPIO_Port, PWM11_Pin, state);
}
void VirPwm_SetOutHandler12(uint8_t state)
{
    HAL_GPIO_WritePin(PWM12_GPIO_Port, PWM12_Pin, state);
}
void VirPwm_SetOutHandler13(uint8_t state)
{
    HAL_GPIO_WritePin(PWM13_GPIO_Port, PWM13_Pin, state);
}
void VirPwm_SetOutHandler14(uint8_t state)
{
    HAL_GPIO_WritePin(PWM14_GPIO_Port, PWM14_Pin, state);
}
void VirPwm_SetOutHandler15(uint8_t state)
{
    HAL_GPIO_WritePin(PWM15_GPIO_Port, PWM15_Pin, state);
}
void VirPwm_SetOutHandler16(uint8_t state)
{
    HAL_GPIO_WritePin(PWM16_GPIO_Port, PWM16_Pin, state);
}
void VirPwm_SetOutHandler17(uint8_t state)
{
    HAL_GPIO_WritePin(PWM17_GPIO_Port, PWM17_Pin, state);
}
void VirPwm_SetOutHandler18(uint8_t state)
{
    HAL_GPIO_WritePin(PWM18_GPIO_Port, PWM18_Pin, state);
}
void VirPwm_SetOutHandler19(uint8_t state)
{
    HAL_GPIO_WritePin(PWM19_GPIO_Port, PWM19_Pin, state);
}
void VirPwm_SetOutHandler20(uint8_t state)
{
    HAL_GPIO_WritePin(PWM20_GPIO_Port, PWM20_Pin, state);
}
void VirPwm_SetOutHandler21(uint8_t state)
{
    HAL_GPIO_WritePin(PWM21_GPIO_Port, PWM21_Pin, state);
}
void VirPwm_SetOutHandler22(uint8_t state)
{
    HAL_GPIO_WritePin(PWM22_GPIO_Port, PWM22_Pin, state);
}
void VirPwm_SetOutHandler23(uint8_t state)
{
    HAL_GPIO_WritePin(PWM23_GPIO_Port, PWM23_Pin, state);
}
void VirPwm_SetOutHandler24(uint8_t state)
{
    HAL_GPIO_WritePin(PWM24_GPIO_Port, PWM24_Pin, state);
}
void VirPwm_SetOutHandler25(uint8_t state)
{
    HAL_GPIO_WritePin(PWM25_GPIO_Port, PWM25_Pin, state);
}
void VirPwm_SetOutHandler26(uint8_t state)
{
    HAL_GPIO_WritePin(PWM26_GPIO_Port, PWM26_Pin, state);
}
void VirPwm_SetOutHandler27(uint8_t state)
{
    HAL_GPIO_WritePin(PWM27_GPIO_Port, PWM27_Pin, state);
}
void VirPwm_SetOutHandler28(uint8_t state)
{
    HAL_GPIO_WritePin(PWM28_GPIO_Port, PWM28_Pin, state);
}
void VirPwm_SetOutHandler29(uint8_t state)
{
    HAL_GPIO_WritePin(PWM29_GPIO_Port, PWM29_Pin, state);
}
void VirPwm_SetOutHandler30(uint8_t state)
{
    HAL_GPIO_WritePin(PWM30_GPIO_Port, PWM30_Pin, state);
}
void VirPwm_SetOutHandler31(uint8_t state)
{
    HAL_GPIO_WritePin(PWM31_GPIO_Port, PWM31_Pin, state);
}


/* USER CODE END Application */
