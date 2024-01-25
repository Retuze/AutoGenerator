#include "HardwareTimer.h"

Timer timer1(&htim1);

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == timer1.htim)
    {
        if (timer1.fun)
        {
            timer1.fun();
        }
    }
}