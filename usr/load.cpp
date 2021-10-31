#include "load.h"

load_class::load_class(const load_cfg_t *const cfg) 
            : cfg(cfg)
{
    for(int i = 0; i < steps; i++) {
        step[i].SetConf(&cfg->step[i]);
        button[i].SetConf(&cfg->button[i], &cfg->button_exti[i]);
    }
}

int load_class::Init(void)
{
    int rv = 0;
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

int load_class::Handler(void)
{
        Set((tm.steps & ~PIN_STEP_1_MASK) | (~tm.steps & PIN_STEP_1_MASK));
        
}