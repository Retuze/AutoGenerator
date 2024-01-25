/*******************************************
# Author        : Retuze
# Date          : 2023-12-26 23:11:55
# LastEditors   : Retuze
# LastEditTime  : 2023-12-26 23:12:01
# Description   :
*********************************************/
#ifndef _CDC_RECIVE_H_
#define _CDC_RECIVE_H_

#ifdef __cplusplus
#include "fifo.h"
extern "C"
{
#endif

    void cdc_fifo_put(uint8_t *p_source, uint32_t len);

#ifdef __cplusplus
}
extern FIFO cdc_fifo;
#endif

#endif // _CDC_RECIVE_H_



