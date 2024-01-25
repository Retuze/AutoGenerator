/*******************************************
# Author        : Retuze 
# Date          : 2024-01-16 21:05:22
# LastEditors   : Retuze 
# LastEditTime  : 2024-01-17 16:29:53
# Description   : 
*********************************************/
#ifndef _SOFT_SPI_H_
#define _SOFT_SPI_H_

#include "stdint.h"
#include "Arduino.h"
#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

uint8_t spi_transfer(uint8_t txData);

#ifdef __cplusplus
}
#endif

#endif // _SOFT_SPI_H_