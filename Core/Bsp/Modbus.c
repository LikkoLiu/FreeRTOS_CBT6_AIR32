#include "Modbus.h"
#include "usbd_cdc_if.h"
uint16_t startRegAddr;
uint16_t RegNum;
uint16_t calCRC;

/**
1. 功能：根据ModBus规则计算CRC16
2. 参数：
3.       _pBuf:待计算数据缓冲区,计算得到的结果存入_pBuf的最后两字节
4.       _usLen:待计算数据长度(字节数)
5. 返回值：16位校验值
*/
static unsigned short int getModbusCRC16(unsigned char *_pBuf, unsigned short int _usLen)
{
    unsigned short int CRCValue = 0xFFFF; // 初始化CRC变量各位为1
    unsigned char i, j;

    for (i = 0; i < _usLen; ++i)
    {
        CRCValue ^= *(_pBuf + i); // 当前数据异或CRC低字节
        for (j = 0; j < 8; ++j)   // 一个字节重复右移8次
        {
            if ((CRCValue & 0x01) == 0x01) // 判断右移前最低位是否为1
            {
                CRCValue = (CRCValue >> 1) ^ 0xA001; // 如果为1则右移并异或表达式
            }
            else
            {
                CRCValue >>= 1; // 否则直接右移一位
            }
        }
    }
    return CRCValue;
}

uint16_t FLASH_data[2];
uint32_t *Modbus_InputIO[128];  // 输入开关量寄存器指针(这里使用的是"位带"操作)
uint32_t *Modbus_OutputIO[128]; // 输出开关量寄存器指针(这里使用的是"位带"操作)
uint16_t *Modbus_HoldReg[32];  // 保持寄存器指针
uint16_t *Modbus_InputReg[32]; // 输入寄存器指针
uint32_t testData1 = 0x0100, testData2 = 0x0200, testData3 = 300, testData4 = 400;
uint32_t testData5 = 500, testData6 = 600, testData7 = 700, testData8 = 800;
uint32_t testData9 = 500, testData10 = 600, testData11 = 700, testData12 = 800;
uint32_t testData13 = 500, testData14 = 600, testData15 = 700, testData16 = 800;
uint32_t testData17 = 500, testData18 = 600, testData19 = 700, testData20 = 800;
uint32_t testData21 = 500, testData22 = 600, testData23 = 700, testData24 = 800;
uint32_t testData25 = 500, testData26 = 600, testData27 = 700, testData28 = 800;
uint32_t testData29 = 500, testData30 = 600, testData31 = 700, testData32 = 800;
/**
 1. 功能：Modbus寄存器和STM32单片机寄存器的映射关系
 2. 参数：无
 3. 返回值：无
*/
void Modbus_RegMap(void)
{
    // 保持寄存器指针指向
    Modbus_HoldReg[0] = (uint16_t *)&testData1;     // 测试数据1
    Modbus_HoldReg[1] = (uint16_t *)&testData2;     // 测试数据2
    Modbus_HoldReg[2] = (uint16_t *)&testData3;     // 测试数据3
    Modbus_HoldReg[3] = (uint16_t *)&testData4;     // 测试数据4
    Modbus_HoldReg[4] = (uint16_t *)&testData5;     // 测试数据5
    Modbus_HoldReg[5] = (uint16_t *)&testData6;     // 测试数据6
    Modbus_HoldReg[6] = (uint16_t *)&testData7;     // 测试数据7
    Modbus_HoldReg[7] = (uint16_t *)&testData8;     // 测试数据8
    Modbus_HoldReg[8] = (uint16_t *)&testData9;     // 测试数据9
    Modbus_HoldReg[9] = (uint16_t *)&testData10;    // 测试数据10
    Modbus_HoldReg[10] = (uint16_t *)&testData11;   // 测试数据11
    Modbus_HoldReg[11] = (uint16_t *)&testData12;   // 测试数据12
    Modbus_HoldReg[12] = (uint16_t *)&testData13;   // 测试数据13
    Modbus_HoldReg[13] = (uint16_t *)&testData14;   // 测试数据14
    Modbus_HoldReg[14] = (uint16_t *)&testData15;   // 测试数据15
    Modbus_HoldReg[15] = (uint16_t *)&testData16;   // 测试数据16
    Modbus_HoldReg[16] = (uint16_t *)&testData17;   // 测试数据17
    Modbus_HoldReg[17] = (uint16_t *)&testData18;   // 测试数据18
    Modbus_HoldReg[18] = (uint16_t *)&testData19;   // 测试数据19
    Modbus_HoldReg[19] = (uint16_t *)&testData20;   // 测试数据20
    Modbus_HoldReg[20] = (uint16_t *)&testData21;   // 测试数据21
    Modbus_HoldReg[21] = (uint16_t *)&testData22;   // 测试数据22
    Modbus_HoldReg[22] = (uint16_t *)&testData23;   // 测试数据23
    Modbus_HoldReg[23] = (uint16_t *)&testData24;   // 测试数据24
    Modbus_HoldReg[24] = (uint16_t *)&testData25;   // 测试数据25
    Modbus_HoldReg[25] = (uint16_t *)&testData26;   // 测试数据26
    Modbus_HoldReg[26] = (uint16_t *)&testData27;   // 测试数据27
    Modbus_HoldReg[27] = (uint16_t *)&testData28;   // 测试数据28
    Modbus_HoldReg[28] = (uint16_t *)&testData29;   // 测试数据29
    Modbus_HoldReg[29] = (uint16_t *)&testData30;   // 测试数据30
    Modbus_HoldReg[30] = (uint16_t *)&testData31;   // 测试数据31
    Modbus_HoldReg[31] = (uint16_t *)&testData32;   // 测试数据32
}

void RS485_Service(void)
{
    // RS485_Addr=(u8)FLASH_data[1];//读取FLASH里设备地址
    uint16_t recCRC;
    if (RS485_FrameFlag == 1)
    {
        if (RS485_RX_BUFF[0] == 0x03) // 地址正确
        {
            if ((RS485_RX_BUFF[1] == 01) || (RS485_RX_BUFF[1] == 02) || (RS485_RX_BUFF[1] == 03) || (RS485_RX_BUFF[1] == 04) || (RS485_RX_BUFF[1] == 05) || (RS485_RX_BUFF[1] == 06) || (RS485_RX_BUFF[1] == 15) || (RS485_RX_BUFF[1] == 16)) // 功能码正确
            {
                startRegAddr = (((uint16_t)RS485_RX_BUFF[2]) << 8) | RS485_RX_BUFF[3]; // 获取寄存器起始地址
                if (startRegAddr < REG_MAXNUM)                                               // 寄存器地址在范围内
                {
                    calCRC = getModbusCRC16(RS485_RX_BUFF, RS485_RX_CNT - 2);                                      // 计算所接收数据的CRC
                    recCRC = RS485_RX_BUFF[RS485_RX_CNT - 2] | (((uint16_t)RS485_RX_BUFF[RS485_RX_CNT - 1]) << 8); // 接收到的CRC(低字节在前，高字节在后)

                    if (calCRC == recCRC) // CRC校验正确
                    {
                        // toggleLED();
                        switch (RS485_RX_BUFF[1]) // 根据不同的功能码进行处理
                        {
                        case 01: // 读输出开关量
                        {
                            // Modbus_01_Solve();
                            break;
                        }

                        case 02: // 读输入开关量
                        {
                            // Modbus_02_Solve();
                            break;
                        }

                        case 03: // 读多个保持寄存器
                        {
                            Modbus_03_Solve();
                            break;
                        }
                        case 04: // 读多个输入寄存器
                        {
                            // Modbus_04_Solve();
                            break;
                        }
                        case 05: // 写单个输出开关量
                        {
                            // Modbus_05_Solve();
                            break;
                        }

                        case 06: // 写单个保持寄存器
                        {
                            Modbus_06_Solve();
                            break;
                        }
                        case 15: // 写多个输出开关量
                        {
                            // Modbus_15_Solve();
                            break;
                        }

                        case 16: // 写多个保持寄存器
                        {
                            // Modbus_16_Solve();
                            break;
                        }
                        }
                        //
                    }
                    else // CRC校验错误
                    {
                        RS485_TX_BUFF[0] = RS485_RX_BUFF[0];
                        RS485_TX_BUFF[1] = RS485_RX_BUFF[1] | 0x80;
                        RS485_TX_BUFF[2] = 0x04; // 异常码
                        CDC_Transmit_FS(RS485_TX_BUFF, 3);
                        // RS485_SendData(RS485_TX_BUFF, 3);
                    }
                }
                else // 寄存器地址超出范围
                {
                    RS485_TX_BUFF[0] = RS485_RX_BUFF[0];
                    RS485_TX_BUFF[1] = RS485_RX_BUFF[1] | 0x80;
                    RS485_TX_BUFF[2] = 0x02; // 异常码
                    CDC_Transmit_FS(RS485_TX_BUFF, 3);
                    // RS485_SendData(RS485_TX_BUFF, 3);
                }
            }
            else // 功能码错误
            {
                RS485_TX_BUFF[0] = RS485_RX_BUFF[0];
                RS485_TX_BUFF[1] = RS485_RX_BUFF[1] | 0x80;
                RS485_TX_BUFF[2] = 0x01; // 异常码
                CDC_Transmit_FS(RS485_TX_BUFF, 3);
                // RS485_SendData(RS485_TX_BUFF, 3);
            }
        }

        RS485_FrameFlag = 0; // 复位帧结束标志
        RS485_RX_CNT = 0;    // 接收计数器清零
        // RS485_TX_EN=0;//开启接收模式
        // GPIO_ResetBits(GPIOA,GPIO_Pin_9);//默认接收状态
    }
}

// Modbus功能码03处理程序
// 读保持寄存器
void Modbus_03_Solve(void)
{
    uint8_t i;
    RegNum = (((uint16_t)RS485_RX_BUFF[4]) << 8) | RS485_RX_BUFF[5]; // 获取寄存器数量
    if ((startRegAddr + RegNum) < REG_MAXNUM)                              // 寄存器地址+数量在范围内
    {
        RS485_TX_BUFF[0] = RS485_RX_BUFF[0];
        RS485_TX_BUFF[1] = RS485_RX_BUFF[1];
        RS485_TX_BUFF[2] = RegNum * 2;
        for (i = 0; i < RegNum; i++)
        {
            RS485_TX_BUFF[3 + i * 2] = (*Modbus_HoldReg[startRegAddr + i] >> 8) & 0xFF; // 先发送高字节
            RS485_TX_BUFF[4 + i * 2] = *Modbus_HoldReg[startRegAddr + i] & 0xFF;        // 后发送低字节
        }
        calCRC = getModbusCRC16(RS485_TX_BUFF, RegNum * 2 + 3);
        RS485_TX_BUFF[RegNum * 2 + 3] = calCRC & 0xFF;
        RS485_TX_BUFF[RegNum * 2 + 4] = (calCRC >> 8) & 0xFF;
        CDC_Transmit_FS(RS485_TX_BUFF, RegNum * 2 + 5);
    }
    else // 寄存器地址+数量超出范围
    {
        RS485_TX_BUFF[0] = RS485_RX_BUFF[0];
        RS485_TX_BUFF[1] = RS485_RX_BUFF[1] | 0x80;
        RS485_TX_BUFF[2] = 0x02; // 异常码
        CDC_Transmit_FS(RS485_TX_BUFF, 3);
    }
}

// Modbus功能码06处理程序
// 写单个保持寄存器
void Modbus_06_Solve(void)
{
    *Modbus_HoldReg[startRegAddr] = RS485_RX_BUFF[4] << 8;         // 高字节在前        修改为高字节在前，低字节在后
    *Modbus_HoldReg[startRegAddr] |= ((uint16_t)RS485_RX_BUFF[5]); // 低字节在后
    RS485_TX_BUFF[0] = RS485_RX_BUFF[0];
    RS485_TX_BUFF[1] = RS485_RX_BUFF[1];
    RS485_TX_BUFF[2] = RS485_RX_BUFF[2];
    RS485_TX_BUFF[3] = RS485_RX_BUFF[3];
    RS485_TX_BUFF[4] = RS485_RX_BUFF[4];
    RS485_TX_BUFF[5] = RS485_RX_BUFF[5];

    calCRC = getModbusCRC16(RS485_TX_BUFF, 6);
    RS485_TX_BUFF[6] = calCRC & 0xFF;
    RS485_TX_BUFF[7] = (calCRC >> 8) & 0xFF;
    CDC_Transmit_FS(RS485_TX_BUFF, 8);
}