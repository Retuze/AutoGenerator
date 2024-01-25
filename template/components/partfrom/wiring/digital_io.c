/*******************************************
# Author        : Retuze 
# Date          : 2024-01-18 08:55:21
# LastEditors   : Retuze 
# LastEditTime  : 2024-01-18 09:10:02
# Description   : 
*********************************************/
#include "digital_io.h"

void digitalWrite(uint32_t ulPin, uint32_t ulVal)
{
  HAL_GPIO_WritePin(GET_GPIO_PORT(ulPin),GET_GPIO_PIN(ulPin),(GPIO_PinState)ulVal);
}

int digitalRead(uint32_t ulPin)
{
  return (int)HAL_GPIO_ReadPin(GET_GPIO_PORT(ulPin),GET_GPIO_PIN(ulPin));
}

void digitalToggle(uint32_t ulPin)
{
  HAL_GPIO_TogglePin(GET_GPIO_PORT(ulPin),GET_GPIO_PIN(ulPin));
}