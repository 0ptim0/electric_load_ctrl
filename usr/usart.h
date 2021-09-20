#pragma once
#include "stm32_base.h"
#include "rcc_periph.h"
#include "gpio.h"

struct usart_cfg_t {
    USART_TypeDef *USART;
    const gpio_cfg_t *tx_cfg;
    const gpio_cfg_t *rx_cfg;
    int speed;
    int timeout;
};

class usart_class : public rcc_periph_class
{
private:
    const usart_cfg_t *const cfg;
    UART_HandleTypeDef USART_InitStructure;
private:
    gpio_class TX;
    gpio_class RX;
public:
    usart_class(const usart_cfg_t *const cfg);
    int Init(void);
    int ClockEnable(void);
    int Transmit(uint8_t *pdata, uint16_t length);
    int Receive(uint8_t *pdata, uint16_t length);
    int Handler(void);
};
