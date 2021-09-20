#pragma once
#include "stm32_base.h"

void float2digits(float number, uint8_t *dig, uint8_t precision, uint8_t digits);
void float2char(float number, char *data, int prec);
void float2uint8(float *number, uint8_t *buf);