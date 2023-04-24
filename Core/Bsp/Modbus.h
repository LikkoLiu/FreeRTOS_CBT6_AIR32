#ifndef __MODBUS_H
#define __MODBUS_H

#include "main.h"
#include "cmsis_os.h"
#include "stm32f1xx_hal.h"
// #include "usbd_cdc_if.h"

#define REG_MAXNUM  30  //寄存器总数

void RS485_Service(void);
void Modbus_03_Solve(void);
void Modbus_06_Solve(void);
void Modbus_10_Solve(void);


#endif /* __LED_H */