#pragma once
#include "stm32_conf.h"

extern usart_class usart;
extern load_class load;

extern "C" 
{
    void USART2_IRQHandler(void) 
    {
        usart.Handler();
    }
    void EXTI0_IRQHandler(void) 
    {
        load.Handler();
    }
    void EXTI1_IRQHandler(void) 
    {
        load.Handler();
    }
    void EXTI2_IRQHandler(void) 
    {
        load.Handler();
    }
    void EXTI3_IRQHandler(void) 
    {
        load.Handler();
    }
    void EXTI4_IRQHandler(void) 
    {
        load.Handler();
    }
    void EXTI9_5_IRQHandler(void) 
    {
        load.Handler();
    }
    void EXTI15_10_IRQHandler(void) 
    {
        load.Handler();
    }
}