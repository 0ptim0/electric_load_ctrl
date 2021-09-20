#include "load.h"

load_class::load_class(const load_cfg_t *const cfg) 
            : cfg(cfg)
{
    for(int i = 0; i < steps; i++) {
        step[i].SetConf(&cfg->step[i]);
    }
}

int load_class::Init(void)
{
    int rv = 0;
    for(int i = 0; i < steps; i++) {
        rv |= step[i].Init();
    }
    return rv;
}

int load_class::Off(void)
{
    for(int i = 0; i < steps; i++) {
        step[i].SetOff();
    }
    arm = DISARM;
    return 0;
}

int load_class::On(void)
{
    arm = ARM;
    return 0;
}

int load_class::Set(uint16_t mask)
{
    if(arm != ARM) return LOAD_OFF;
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