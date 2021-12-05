#include "load.h"

load_class::load_class(load_cfg_t *cfg) 
            : cfg(cfg)
{
}

int load_class::Init(void)
{
    int rv = 0;
    for(int i = 0; i < steps; i++) {
        step[i].SetConf(&cfg->step[i]);
        step[i].SetOff();
        button[i].SetConf(&cfg->button[i], &cfg->button_exti[i]);
    }
    for(int i = 0; i < steps; i++) {
        rv |= step[i].Init();
        rv |= button[i].Init();
    }
    return rv;
}

int load_class::Off(void)
{
    for(int i = 0; i < steps; i++) {
        step[i].SetOff();
    }
    return 0;
}

int load_class::On(void)
{
    return 0;
}

int load_class::Set(uint16_t mask)
{
    tm.steps = mask;
    for(int i = 0; i < steps; i++) {
        if(mask & (1 << i)) {
            step[i].SetOn();
        }
        else {
            step[i].SetOff();
        }
    }
    return 0;
}

int load_class::IT(uint32_t pin)
{
    if(pin > LOAD_STEPS) return EINVAL;
    button[pin].ClearBitEXTI_IT();
    return 0;
}

int load_class::Handle(void)
{
    uint16_t num;
    uint16_t mask;

    if(tm.state != MANUAL && tm.state != MIXED) return 0;

    mask = EXTI->PR;
    num = GetNumFromMask(mask);
    if(num > LOAD_STEPS + 1) return EINVAL;

    for(int i = 0; i < LOAD_STEPS; i++) {
        if(button[i].GetPin() == (mask & (0x1 << num))) {
            num = i;
            break;
        }
    }

    mask = 0x1 << num;

    Set((tm.steps & ~mask) | (~tm.steps & mask));
    button[num].ClearBitEXTI_IT();
    return 0;
}