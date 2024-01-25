/*******************************************
# Author        : Retuze
# Date          : 2024-01-18 09:46:22
# LastEditors   : Retuze
# LastEditTime  : 2024-01-18 09:46:22
# Description   :
*********************************************/
#ifndef _WIRING_TIME_H_
#define _WIRING_TIME_H_

#include "partfrom.h"

#ifdef __cplusplus
extern "C"
{
#endif

    uint32_t millis(void);
    uint32_t micros(void);
    void delay(uint32_t ms);
    void delayMicroseconds(uint32_t us);

#ifdef __cplusplus
}
#endif

#endif // _WIRING_TIME_H_