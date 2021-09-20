#pragma once
#include "stm32_base.h"

class rcc_periph_class
{
public:
    virtual int ClockEnable(void) = 0;
};