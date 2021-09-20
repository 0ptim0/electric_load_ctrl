#include "stm32_base.h"
#include "gpio.h"

gpio_class::gpio_class() : cfg(nullptr) 
{
}

gpio_class::gpio_class(const gpio_cfg_t *const cfg) : cfg(cfg) 
{
}

int gpio_class::Init(void) 
{   
    int rv;
    if(rv = this->ClockEnable() != 0) return rv;
    if(cfg != nullptr) {
        HAL_GPIO_Init(cfg->GPIO, (GPIO_InitTypeDef *)&cfg->GPIO_InitStructure);
    } else {
        return EINVAL;
    }
    return 0;
}

int gpio_class::SetConf(const gpio_cfg_t *const cfg) 
{   
    int rv;
    if(cfg != nullptr) {
        HAL_GPIO_Init(cfg->GPIO, (GPIO_InitTypeDef *)&cfg->GPIO_InitStructure);
    } else {
        return EINVAL;
    }
    return 0;
}

void gpio_class::SetOn(void) 
{
    HAL_GPIO_WritePin(cfg->GPIO, cfg->GPIO_InitStructure.Pin, GPIO_PIN_SET);
}

void gpio_class::SetOff(void) 
{   
    HAL_GPIO_WritePin(cfg->GPIO, cfg->GPIO_InitStructure.Pin, GPIO_PIN_RESET);
}

int gpio_class::ClockEnable(void)
{
    switch(reinterpret_cast<uint32_t>(cfg->GPIO)) {

#ifdef GPIOA
        case GPIOA_BASE:
            __HAL_RCC_GPIOA_CLK_ENABLE();
            break;
#endif

#ifdef GPIOB
        case GPIOB_BASE:
            __HAL_RCC_GPIOB_CLK_ENABLE();
            break;
#endif

#ifdef GPIOC
        case GPIOC_BASE:
            __HAL_RCC_GPIOC_CLK_ENABLE();
            break;
#endif

#ifdef GPIOD
        case GPIOD_BASE:
            __HAL_RCC_GPIOD_CLK_ENABLE();
            break;
#endif

#ifdef GPIOE
        case GPIOE_BASE:
            __HAL_RCC_GPIOE_CLK_ENABLE();
            break;
#endif

#ifdef GPIOF
        case GPIOF_BASE:
            __HAL_RCC_GPIOF_CLK_ENABLE();
            break;
#endif

#ifdef GPIOG
        case GPIOG_BASE:
            __HAL_RCC_GPIOG_CLK_ENABLE();
            break;
#endif

#ifdef GPIOH
        case GPIOH_BASE:
            __HAL_RCC_GPIOH_CLK_ENABLE();
            break;
#endif

        default:
            return EINVAL;
    }

    return 0;
}

