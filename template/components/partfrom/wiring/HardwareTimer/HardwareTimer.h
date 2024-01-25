#ifndef _HARDWARETIMER_H_
#define _HARDWARETIMER_H_

#include "tim.h"

#ifdef __cplusplus
extern "C"
{
#endif

    void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

#ifdef __cplusplus
}

class Timer
{
private:
public:
    void (*fun)();
    TIM_HandleTypeDef *htim;
    Timer(TIM_HandleTypeDef *htim)
    {
        this->htim = htim;
    }
    void begin()
    {
        HAL_TIM_Base_Start(htim);
    }
    void begin_with_interrupt()
    {
        HAL_TIM_Base_Start_IT(htim);
    }
    void begin_with_pwm(uint32_t Channel)
    {
        HAL_TIM_PWM_Start(htim, Channel);
    }
    /// @brief 设置定时器频率
    /// @param freq
    void freq(uint32_t freq)
    {
        float duty1 = (float)(*(&(htim->Instance->CCR1)) * 100) / htim->Instance->ARR;
        float duty2 = (float)(*(&(htim->Instance->CCR2)) * 100) / htim->Instance->ARR;
        float duty3 = (float)(*(&(htim->Instance->CCR3)) * 100) / htim->Instance->ARR;
        float duty4 = (float)(*(&(htim->Instance->CCR4)) * 100) / htim->Instance->ARR;
        // freq = sys_freq / (PSC + 1) / (ARR + 1);
        uint32_t temp = HAL_RCC_GetSysClockFreq() / freq;
        for (uint32_t i = 1; i <= temp; i++)
        {
            static uint32_t last;
            if (temp % i == 0)
            {
                if (i > 65536)
                {
                    htim->Instance->ARR = last - 1;
                    break;
                }
                htim->Instance->ARR = i - 1;
                last = i;
            }
        }
        htim->Instance->PSC = temp / (htim->Instance->ARR + 1) - 1;
        *(&(htim->Instance->CCR1)) = (uint32_t)(htim->Instance->ARR * duty1 / 100);
        *(&(htim->Instance->CCR2)) = (uint32_t)(htim->Instance->ARR * duty2 / 100);
        *(&(htim->Instance->CCR3)) = (uint32_t)(htim->Instance->ARR * duty3 / 100);
        *(&(htim->Instance->CCR4)) = (uint32_t)(htim->Instance->ARR * duty4 / 100);
    }
    void duty(uint32_t Channel, float duty)
    {
        *(&(htim->Instance->CCR1) + (Channel / 4 - 1)) = (float)(htim->Instance->ARR * duty / 100);
    }
    void attachInterrupt(void (*fun)())
    {
        this->fun = fun;
    }
};

extern Timer timer1;

#endif


#endif // _HARDWARETIMER_H_


