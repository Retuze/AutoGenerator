/*******************************************
# Author        : Retuze 
# Date          : 2024-01-09 02:03:59
# LastEditors   : Retuze 
# LastEditTime  : 2024-01-11 04:11:30
# Description   : 
*********************************************/
#include "serial.h"

UART_RECIVE uart_re(&huart1);

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if(huart==&huart1)
    {
        uart_re.run(huart,Size);
    }
}

Serial serial([](uint8_t *buf,uint16_t len){
    HAL_UART_Transmit(&huart1, (const uint8_t *)buf, len, 100);
});

// Serial usb_serial([](uint8_t *buf,uint16_t len){
//     CDC_Transmit_FS((uint8_t *)buf,len);
// });