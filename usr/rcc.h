#pragma once
#include "stm32_base.h"

class rcc_class
{
private:
    RCC_ClkInitTypeDef RCC_ClkInitStructure;
    RCC_OscInitTypeDef RCC_OscInitStructure;
public:
    void InitClock();
};