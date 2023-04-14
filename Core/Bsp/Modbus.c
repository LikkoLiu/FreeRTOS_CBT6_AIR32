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
                if (startRegAddr < 1000)                                               // 寄存器地址在范围内
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
                            // Modbus_03_Solve();
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

// Modbus功能码06处理程序
// 写单个保持寄存器
void Modbus_06_Solve(void)
{
    // *Modbus_HoldReg[startRegAddr]=RS485_RX_BUFF[4]<<8;//高字节在前        修改为高字节在前，低字节在后
    // *Modbus_HoldReg[startRegAddr]|=((u16)RS485_RX_BUFF[5]);//低字节在后
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
    // if (startRegAddr == 8 || startRegAddr == 9)
    //     FLASH_WriteMoreData(0x800FC00, FLASH_data, 2);
}