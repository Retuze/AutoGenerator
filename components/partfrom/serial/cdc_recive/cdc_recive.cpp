/*******************************************
# Author        : Retuze
# Date          : 2023-12-26 23:06:41
# LastEditors   : Retuze
# LastEditTime  : 2023-12-26 23:07:56
# Description   :
*********************************************/
#include "cdc_recive.h"

FIFO cdc_fifo(256);

void cdc_fifo_put(uint8_t *p_source, uint32_t len)
{
    cdc_fifo.put(p_source,len);
}