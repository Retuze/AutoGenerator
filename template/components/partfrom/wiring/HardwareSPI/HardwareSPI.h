#ifndef _HARDWARESPI_H_
#define _HARDWARESPI_H_

#include "partfrom.h"

#ifdef __cplusplus
extern "C" {
#endif



#ifdef __cplusplus
}

class SPIClass
{
private:
    SPI_HandleTypeDef *hspi;
public:
    SPIClass(SPI_HandleTypeDef *hspi)
    {
        this->hspi = hspi;
    }
    uint8_t transfer(uint8_t TxData)
    {
        uint8_t RxData = 0;
        HAL_SPI_TransmitReceive(hspi,&TxData,&RxData,1,100);
        return RxData;
    }
};

extern SPIClass ASPI;

#endif

#endif // _HARDWARESPI_H_
