/*******************************************
# Author        : Retuze
# Date          : 2024-01-11 04:33:09
# LastEditors   : Retuze
# LastEditTime  : 2024-01-17 17:14:06
# Description   :
*********************************************/

#include "app.h"
#include "led.h"
#include "Arduino.h"
#include "shell.h"
#include "prefs.h"
#include "task.h"
#include "partfrom.h"
#include "userCmd.h"
#include "soft_spi.h"
#include "MFRC522.h"

MFRC522 mfrc522;

LEDC led(
    [](uint8_t val)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, (GPIO_PinState)(val / 100));
    });

void app_lunch(void)
{
    ulog("欢迎！！！\n");
    Serial.begin();
    Serial.attachCallback(
        [](char *str)
        {
            shell.exec(str);
        }
    );

    timer1.begin_with_interrupt();
    timer1.freq(1000);
    timer1.attachInterrupt(
        []()
        {
            led.run();
            Serial.run();
        });

    RegisterCommand();

    led.blink(5000);

    mfrc522.PCD_Init(
        [](byte val) -> byte
        {
            return ASPI.transfer(val);
        },
        [](byte msg, byte val) -> byte
        {
            switch (msg)
            {
            case MFRC522::_delayMs:
                HAL_Delay(val);
                break;
            case MFRC522::_chipSelectPin:
                HAL_GPIO_WritePin(RC522_CS_GPIO_Port, RC522_CS_Pin, (GPIO_PinState)val);
                break;
            case MFRC522::_resetPowerDownPin:
                HAL_GPIO_WritePin(RC522_RST_GPIO_Port, RC522_RST_Pin, (GPIO_PinState)val);
                break;
            default:
                break;
            }
            return 0;
        },
        []() -> uint32_t
        {
            return uwTick;
        });

    mfrc522.PCD_DumpVersionToSerial(); // Show details of PCD - MFRC522 Card Reader details
    Serial.println(("Scan PICC to see UID, SAK, type, and data blocks..."));
    while (1)
    {
        task.run();
    }
}