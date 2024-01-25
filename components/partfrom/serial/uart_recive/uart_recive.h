/*******************************************
# Author        : Retuze
# Date          : 2023-12-02 00:18:24
# LastEditors   : Retuze 
# LastEditTime  : 2024-01-10 10:11:27
# Description   :
*********************************************/
#ifndef _UART_RECIVE_H_
#define _UART_RECIVE_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "usart.h"

#include "string.h"
#include "stdlib.h"
#include "stdint.h"
#include "fifo.h"

    // void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);

#ifdef __cplusplus
}
#endif

class UART_RECIVE
{
public:
    UART_RECIVE(UART_HandleTypeDef *huart)
    {
        this->huart = huart;
    }
    uint8_t Rx_buf_pos; // 本次回调接收的数据在缓冲区的起点
    uint8_t Rx_length;  // 本次回调接收数据的长度
    UART_HandleTypeDef *huart;
    class uart_data
    {
    public:
        uint32_t buffer_size;
        FIFO *fifo;
        uint8_t *buffer;
        uint8_t *dma_buffer;
        uart_data(uint32_t buffer_size = 256)
        {
            fifo = new FIFO(buffer_size);
            buffer = (uint8_t *)malloc(buffer_size);
            dma_buffer = (uint8_t *)malloc(buffer_size);
            this->buffer_size = buffer_size;
        }
    };
    uart_data data = uart_data();
    uint32_t GetDataLength()
    {
        return data.fifo->len();
    }
    // 建议至少20ms取一次数据
    const char *GetData()
    {
        if (data.fifo->len()==0)
        {
            return NULL;
        }
        else
        {
            memset(data.buffer, 0, data.buffer_size);
            data.fifo->get(data.buffer, data.fifo->len());
            return (const char *)data.buffer;
        }
    }
    void begin()
    {
        HAL_UARTEx_ReceiveToIdle_DMA(huart, data.dma_buffer, data.buffer_size);
    }
    void run(UART_HandleTypeDef *huart, uint16_t Size)
    {
        if (huart == this->huart)
        {
            Rx_length = Size - Rx_buf_pos;
            data.fifo->put(&data.dma_buffer[Rx_buf_pos], Rx_length);
            Rx_buf_pos += Rx_length;
            if (Rx_buf_pos >= data.buffer_size)
                Rx_buf_pos = 0; // 缓冲区用完后，返回 0 处重新开始
        }
    }
};

#endif // _UART_RECIVE_H_