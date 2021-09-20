#include "stm32_base.h"

void Error_Handler() {
    __disable_irq();
    while(1);
}