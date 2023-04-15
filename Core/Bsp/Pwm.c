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
		.count_N = 100
};


/**
 * @brief 完成初始化VirPwmDef结构体指针
 * 		也可以直接在定义结构体的时候完成初始化，而不必调用本函数
 * @param VirPwm: 虚拟PWM的定义结构体指针
 * @param freq: 虚拟PWM的频率
 * @param dutyCycle: 虚拟PWM的占空比
 * @param idle: 虚拟PWM的空闲状态
 */
void VirPwm_Init(VirPwm *VirPwmDef,uint16_t freq,uint16_t dutyCycle,uint8_t idle, VirPwm_SetOutput setOutHandler)
{
	VirPwm_SetFreq(VirPwmDef,freq);
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
	//0~100 duty cycle
	uint32_t timer_freq;

	//Range of frequency is 1 to 1000.
	if (freq > 1000)
	{
		freq = 1000;
	}

	if(freq < 1)
	{
		freq = 1;
	}

	VirPwmDef->Frequency = freq;

	timer_freq = VirPwmDef->Frequency*100;			//100~100k

	//timer_freq < 1k, psc = 7200, f = 10kHz 100*100 Hz (freq < 10)
	if(timer_freq<1000)
	{
		VirPwmDef->Basetimer->PSC = 7200 - 1;
	}
	//timer_freq < 10k, psc = 720, f = 100kHz 1k*100 Hz (freq < 100)
	else if(timer_freq <10000)
	{
		VirPwmDef->Basetimer->PSC = 720 - 1;
	}
	//timer_freq < 100k, psc = 72, f = 1MHz 10k*100 Hz (freq < 1000)
	else
	{
		VirPwmDef->Basetimer->PSC = 72 - 1;
	}

	//Set the update frequency of the timer.
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
	//Range of duty cycle is 0 to 100.
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
	if(idle != 0)
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
	//占空比为0，则直接关闭PWM输出，将端口设置为idle状态
	if(VirPwmDef->DutyCycle == 0)
	{
		if(VirPwmDef->Idle != 0)
		{
			VirPwmDef->SetOut(1);
		}
		else
		{
			VirPwmDef->SetOut(0);
		}

		return;
	}

	//占空比为100，则直接打开PWM输出，将端口设置为busy状态
	if(VirPwmDef->DutyCycle >= 100)
	{
		if(VirPwmDef->Idle != 0)
		{
			VirPwmDef->SetOut(0);
		}
		else
		{
			VirPwmDef->SetOut(1);
		}

		return;
	}

	//占空比为1~99时，正常处理
	if(VirPwmDef->count_P == 0)
	{
		if(VirPwmDef->count_N == 0)
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
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_13, state);
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
