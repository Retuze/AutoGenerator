/*******************************************
# Author        : Retuze
# Date          : 2024-01-09 02:03:42
# LastEditors   : Retuze 
# LastEditTime  : 2024-01-11 04:12:13
# Description   :
*********************************************/
#ifndef _SERIAL_H_
#define _SERIAL_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdarg.h>
#include "usart.h"
// #include "usbd_cdc_if.h"
#include "partfrom.h"

    void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);

#ifdef __cplusplus
}

class Serial
{
private:
    void (*_write)(uint8_t *buf, uint16_t len);

public:
    Serial(void (*_write)(uint8_t *buf, uint16_t len))
    {
        this->_write = _write;
    }
    int printf(const char *fmt, ...)
    {
        int ret;
        va_list args; // 用于存放参数列表的数据结构
        va_start(args, fmt);
        // 设置输出缓冲区，为256字节大小
        char sprint_buf[256] = {0};
        ret = VSPRINTF(sprint_buf, fmt, args);
        if (_write)
        {
            // 将格式化好的内容输出
            _write((uint8_t *)sprint_buf, ret);
        }
        else
        {
            // 这里应该报个错，但是连printf都没有，报个毛线
        }
        va_end(args); // 执行清理参数列表的工作
        return ret;
    }
};

#include "uart_recive.h"
extern UART_RECIVE uart_re;

extern Serial serial;
// extern Serial usb_serial;

#endif

#endif // _SERIAL_H_