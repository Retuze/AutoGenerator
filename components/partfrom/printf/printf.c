/*******************************************
# Author        : Retuze
# Date          : 2024-01-09 04:01:53
# LastEditors   : Retuze 
# LastEditTime  : 2024-01-09 15:51:55
# Description   : 用于printf重定向
*********************************************/
#include "printf.h"

// gcc 标准printf重定向，默认不支持浮点数打印
int _write(int fd, char *pBuffer, int size)
{
    HAL_UART_Transmit(&huart1, (const uint8_t *)pBuffer, size, 100);
    // CDC_Transmit_FS((uint8_t *)pBuffer, size);
    return size;
}