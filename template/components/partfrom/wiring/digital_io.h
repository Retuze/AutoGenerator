/*******************************************
# Author        : Retuze 
# Date          : 2024-01-18 08:56:00
# LastEditors   : Retuze 
# LastEditTime  : 2024-01-18 09:31:35
# Description   : 
*********************************************/
#ifndef _DIGITAL_IO_H_
#define _DIGITAL_IO_H_

#include "stdint.h"
#include "partfrom.h"

/*----------------------------------------------------------------------------
 *        STM32 pins number
 *----------------------------------------------------------------------------*/
#define PA0                     0
#define PA1                     1
#define PA2                     2
#define PA3                     3
#define PA4                     4
#define PA5                     5
#define PA6                     6
#define PA7                     7
#define PA8                     8
#define PA9                     9
#define PA10                    10
#define PA11                    11
#define PA12                    12
#define PA13                    13
#define PA14                    14
#define PA15                    15
#define PB0                     16
#define PB1                     17
#define PB2                     18
#define PB3                     19
#define PB4                     20
#define PB5                     21
#define PB6                     22
#define PB7                     23
#define PB8                     24
#define PB9                     25
#define PB10                    26
#define PB11                    27
#define PB12                    28
#define PB13                    29
#define PB14                    30
#define PB15                    31
#define PC13                    32
#define PC14                    33
#define PC15                    34
#define PD0                     35
#define PD1                     36

#define GET_GPIO_PORT(PinNum)   (\
      (PinNum / 16) == 0 ? GPIOA \
    : (PinNum / 16) == 1 ? GPIOB \
    : (PinNum / 16) == 2 ? GPIOC \
    : GPIOD                     )

#define GET_GPIO_PIN(PinNum)           (\
      (PinNum % 16) == 0 ? GPIO_PIN_0   \
    : (PinNum % 16) == 1 ? GPIO_PIN_1   \
    : (PinNum % 16) == 2 ? GPIO_PIN_2   \
    : (PinNum % 16) == 3 ? GPIO_PIN_3   \
    : (PinNum % 16) == 4 ? GPIO_PIN_4   \
    : (PinNum % 16) == 5 ? GPIO_PIN_5   \
    : (PinNum % 16) == 6 ? GPIO_PIN_6   \
    : (PinNum % 16) == 7 ? GPIO_PIN_7   \
    : (PinNum % 16) == 8 ? GPIO_PIN_8   \
    : (PinNum % 16) == 9 ? GPIO_PIN_9   \
    : (PinNum % 16) == 10 ? GPIO_PIN_10 \
    : (PinNum % 16) == 11 ? GPIO_PIN_11 \
    : (PinNum % 16) == 12 ? GPIO_PIN_12 \
    : (PinNum % 16) == 13 ? GPIO_PIN_13 \
    : (PinNum % 16) == 14 ? GPIO_PIN_14 \
    : GPIO_PIN_15                       )
     
#ifdef __cplusplus
extern "C" {
#endif

void digitalWrite(uint32_t ulPin, uint32_t ulVal);
int digitalRead(uint32_t ulPin);
void digitalToggle(uint32_t ulPin);

#ifdef __cplusplus
}
#endif

#endif // _DIGITAL_IO_H_

