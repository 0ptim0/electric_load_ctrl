#include "stm32_conf.h"

rcc_class rcc;

int main(void) {
    HAL_Init();
    rcc.InitClock();
    vTaskStartScheduler();
    while(1){  
    }
}
