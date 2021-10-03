#include "stm32_base.h"
#include "gpio.h"
#pragma once

#define LOAD_STEPS 10

#define LOAD_OFF 0x01

#define ARM 1
#define DISARM 0

#define CTRL_COMP_ADDR  0x01
#define LOAD_ADDR       0x64

#define LOAD_CMD_ON         0x01
#define LOAD_CMD_OFF        0x02
#define LOAD_CMD_SET        0x03

struct load_cfg_t {
    const gpio_cfg_t step[LOAD_STEPS];
};

class load_class 
{
private:
    const uint8_t steps = LOAD_STEPS;
    const load_cfg_t *const cfg;
    gpio_class step[LOAD_STEPS];
public:
    uint32_t voltage;
    uint32_t current;
    uint16_t mask;
    bool arm;
public:
    load_class(const load_cfg_t *const cfg);
    int Init(void);
    int Off(void);
    int On(void);
    int Set(uint16_t mask);
};