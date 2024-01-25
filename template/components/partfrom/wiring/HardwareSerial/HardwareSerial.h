#ifndef _HARDWARESERIAL_H_
#define _HARDWARESERIAL_H_

#include "Print.h"
#include "fifo.h"
#include "partfrom.h"

#ifdef __cplusplus
extern "C"
{
#endif

    void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);

#ifdef __cplusplus
}

class HardwareSerial : public Print
{
private:
    FIFO fifo;
    uint8_t _lastDMASize = 0;
    int _lastDATASize = 0;
    void (*fun)(char *); // 接收到数据时的回调函数
    size_t write(const uint8_t *str, size_t size)
    {
        HAL_UART_Transmit(huart, str, size, 100);
        return size;
    }

public:
    UART_HandleTypeDef *huart;
    void begin()
    {
        HAL_UARTEx_ReceiveToIdle_DMA(huart, (uint8_t *)fifo.ring_buf->buffer, fifo.ring_buf->size);
    }
    void get(UART_HandleTypeDef *huart, uint16_t Size)
    {
        if (huart == this->huart)
        {
            // ulog("缓冲区大小：%d\r\n",Size);
            // ulog("上次缓冲区大小：%d\r\n",_lastDMASize);
            // ulog("中断类型：%d\r\n",huart->RxEventType);
            fifo.ring_buf->in += (Size - _lastDMASize);
            (huart->RxEventType == HAL_UART_RXEVENT_TC) ? _lastDMASize = 0 : _lastDMASize = Size;
        }
    }
    HardwareSerial(UART_HandleTypeDef *huart)
    {
        this->huart = huart;
    }
    char *read()
    {
        char *buff = (char *)calloc(fifo.length() + 1, sizeof(char));
        fifo.get(buff, fifo.length());
        return buff;
    }
    char *peek()
    {
        char *buff = (char *)calloc(fifo.length() + 1, sizeof(char));
        fifo.peek(buff, fifo.length());
        return buff;
    }
    int available()
    {
        return fifo.length();
    }
    /// @brief 在两次接收到数据没有发生改变后，认为接收到了完整的命令，执行fun函数
    /// @param fun
    /// @return
    void run()
    {
        if (available()&&available() == _lastDATASize)
        {
            char *str = read();
            fun(str);
            free(str);
        }
        _lastDATASize = available();
    }
    void attachCallback(void (*fun)(char *))
    {
        this->fun = fun;
    }
};

extern HardwareSerial Serial;

#endif

#endif // _HARDWARESERIAL_H_