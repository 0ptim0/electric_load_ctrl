#include "stm32_base.h"
#include "rcc.h"

void rcc_class::InitClock() 
{
    RCC_ClkInitTypeDef RCC_ClkInitStructure;
    RCC_OscInitTypeDef RCC_OscInitStructure;

    RCC_OscInitStructure.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStructure.HSEState = RCC_HSE_ON;
    RCC_OscInitStructure.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStructure.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStructure.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStructure.PLL.PLLMUL = RCC_PLL_MUL9;
    while(HAL_RCC_OscConfig(&RCC_OscInitStructure) != HAL_OK);

    RCC_ClkInitStructure.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                    | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStructure.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStructure.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStructure.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStructure.APB2CLKDivider = RCC_HCLK_DIV1;
    if(HAL_RCC_ClockConfig(&RCC_ClkInitStructure, FLASH_LATENCY_2) != HAL_OK);
}
