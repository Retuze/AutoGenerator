/*******************************************
# Author        : Retuze
# Date          : 2024-01-09 04:02:01
# LastEditors   : Retuze 
# LastEditTime  : 2024-01-11 16:22:55
# Description   : 用于printf重定向
*********************************************/
#ifndef _PRINTF_H_
#define _PRINTF_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdarg.h>

#include "usart.h"
#include "usbd_cdc_if.h"

int _write(int fd, char *pBuffer, int size);

#ifdef __cplusplus
}
#include "fifo.h"
#endif

#endif // _PRINTF_H_