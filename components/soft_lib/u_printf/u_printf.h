/*******************************************
# Author        : Retuze 
# Date          : 2024-01-09 04:30:29
# LastEditors   : Retuze 
# LastEditTime  : 2024-01-09 04:31:57
# Description   : 
*********************************************/
#ifndef _U_PRINTF_H_
#define _U_PRINTF_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>

// 格式化输出，使用同glibc的printf
// 注意使用前需先使用xdev_out设定输出函数
// ---------------------------------
int u_printf(const char *fmt, ...);
int u_vprintf(const char *fmt, va_list args);
int u_vsprintf(char *buf, const char *fmt, va_list args);

// 设置u_printf的输出函数
void xdev_out(void (*func)(const char *str, unsigned char len));

#ifdef __cplusplus
}
#endif

#endif // _U_PRINTF_H_