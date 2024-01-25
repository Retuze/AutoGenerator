/*******************************************
# Author        : Retuze
# Date          : 2024-01-09 04:01:53
# LastEditors   : Retuze 
# LastEditTime  : 2024-01-18 10:46:34
# Description   : 用于printf重定向
*********************************************/
#include "write.h"

FIFO cdc_rx;

// gcc 标准printf重定向，默认不支持浮点数打印
int _write(int fd, char *pBuffer, int size)
{
    char data[128] = {0};
    uint32_t len = 0;
    if (cdc_rx.length() != 0)
    {
        len = cdc_rx.length();
        cdc_rx.get(data, len);
        if (CDC_Transmit_FS((uint8_t *)data, len) != 0)
        {
            cdc_rx.put(data, len);
        }
    }
    HAL_UART_Transmit(&huart1, (const uint8_t *)pBuffer, size, 100);
    // 当没有发送成功时缓存数据等待下一次发送
    if (CDC_Transmit_FS((uint8_t *)pBuffer, size) != 0)
    {
        cdc_rx.put(pBuffer, size);
    }
    return size;
}