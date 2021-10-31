#pragma once
#include "stm32_conf.h"

#define PIN_USART_TX    {GPIOA, {GPIO_PIN_2, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}
#define PIN_USART_RX    {GPIOA, {GPIO_PIN_3, GPIO_MODE_AF_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}

#define PIN_CAN_RX      {GPIOA, {GPIO_PIN_11, GPIO_MODE_AF_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}
#define PIN_CAN_TX      {GPIOA, {GPIO_PIN_12, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}

#define PIN_STEP_1      {GPIOB, {GPIO_PIN_5, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}
#define PIN_STEP_2      {GPIOB, {GPIO_PIN_4, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}
#define PIN_STEP_3      {GPIOB, {GPIO_PIN_3, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}
#define PIN_STEP_4      {GPIOA, {GPIO_PIN_10, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}
#define PIN_STEP_5      {GPIOA, {GPIO_PIN_9, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}
#define PIN_STEP_6      {GPIOA, {GPIO_PIN_8, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}
#define PIN_STEP_7      {GPIOB, {GPIO_PIN_15, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}
#define PIN_STEP_8      {GPIOB, {GPIO_PIN_14, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}
#define PIN_STEP_9      {GPIOB, {GPIO_PIN_13, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}
#define PIN_STEP_10     {GPIOB, {GPIO_PIN_12, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}

#define PIN_BUTTON_1    {GPIOA, {GPIO_PIN_1, GPIO_MODE_IT_FALLING, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}
#define PIN_BUTTON_2    {GPIOA, {GPIO_PIN_4, GPIO_MODE_IT_FALLING, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}
#define PIN_BUTTON_3    {GPIOA, {GPIO_PIN_5, GPIO_MODE_IT_FALLING, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}
#define PIN_BUTTON_4    {GPIOA, {GPIO_PIN_6, GPIO_MODE_IT_FALLING, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}
#define PIN_BUTTON_5    {GPIOA, {GPIO_PIN_7, GPIO_MODE_IT_FALLING, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}
#define PIN_BUTTON_6    {GPIOB, {GPIO_PIN_0, GPIO_MODE_IT_FALLING, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}
#define PIN_BUTTON_7    {GPIOB, {GPIO_PIN_8, GPIO_MODE_IT_FALLING, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}
#define PIN_BUTTON_8    {GPIOB, {GPIO_PIN_2, GPIO_MODE_IT_FALLING, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}
#define PIN_BUTTON_9    {GPIOB, {GPIO_PIN_10, GPIO_MODE_IT_FALLING, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}
#define PIN_BUTTON_10   {GPIOB, {GPIO_PIN_11, GPIO_MODE_IT_FALLING, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}

#define PIN_BUTTON_1    {GPIOA, {GPIO_PIN_1, GPIO_MODE_IT_FALLING, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}
#define PIN_BUTTON_2    {GPIOA, {GPIO_PIN_4, GPIO_MODE_IT_FALLING, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}
#define PIN_BUTTON_3    {GPIOA, {GPIO_PIN_5, GPIO_MODE_IT_FALLING, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}
#define PIN_BUTTON_4    {GPIOA, {GPIO_PIN_6, GPIO_MODE_IT_FALLING, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}
#define PIN_BUTTON_5    {GPIOA, {GPIO_PIN_7, GPIO_MODE_IT_FALLING, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}
#define PIN_BUTTON_6    {GPIOB, {GPIO_PIN_0, GPIO_MODE_IT_FALLING, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}
#define PIN_BUTTON_7    {GPIOB, {GPIO_PIN_8, GPIO_MODE_IT_FALLING, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}
#define PIN_BUTTON_8    {GPIOB, {GPIO_PIN_2, GPIO_MODE_IT_FALLING, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}
#define PIN_BUTTON_9    {GPIOB, {GPIO_PIN_10, GPIO_MODE_IT_FALLING, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}
#define PIN_BUTTON_10   {GPIOB, {GPIO_PIN_11, GPIO_MODE_IT_FALLING, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}

#define PIN_EXTI_1    {GPIOA, {GPIO_PIN_1, GPIO_MODE_IT_FALLING, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}
#define PIN_EXTI_2    {GPIOA, {GPIO_PIN_4, GPIO_MODE_IT_FALLING, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}
#define PIN_EXTI_3    {GPIOA, {GPIO_PIN_5, GPIO_MODE_IT_FALLING, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}
#define PIN_EXTI_4    {GPIOA, {GPIO_PIN_6, GPIO_MODE_IT_FALLING, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}
#define PIN_EXTI_5    {GPIOA, {GPIO_PIN_7, GPIO_MODE_IT_FALLING, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}
#define PIN_EXTI_6    {GPIOB, {GPIO_PIN_0, GPIO_MODE_IT_FALLING, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}
#define PIN_EXTI_7    {GPIOB, {GPIO_PIN_8, GPIO_MODE_IT_FALLING, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}
#define PIN_EXTI_8    {GPIOB, {GPIO_PIN_2, GPIO_MODE_IT_FALLING, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}
#define PIN_EXTI_9    {GPIOB, {GPIO_PIN_10, GPIO_MODE_IT_FALLING, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}
#define PIN_EXTI_10   {GPIOB, {GPIO_PIN_11, GPIO_MODE_IT_FALLING, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}

const gpio_cfg_t tx = PIN_USART_TX;
const gpio_cfg_t rx = PIN_USART_RX;

const gpio_cfg_t can_rx_pin_cfg = PIN_CAN_RX;
const gpio_cfg_t can_tx_pin_cfg = PIN_CAN_TX;

const usart_cfg_t usart_cfg = {USART2, &tx, &rx, 115200, 20};

const load_cfg_t load_cfg = 
{
    {
        PIN_STEP_1, PIN_STEP_2, PIN_STEP_3, PIN_STEP_4, PIN_STEP_5,
        PIN_STEP_6, PIN_STEP_7, PIN_STEP_8, PIN_STEP_9, PIN_STEP_10
    },
    {
        PIN_BUTTON_1, PIN_BUTTON_2, PIN_BUTTON_3, PIN_BUTTON_4, PIN_BUTTON_5,
        PIN_BUTTON_6, PIN_BUTTON_7, PIN_BUTTON_8, PIN_BUTTON_9, PIN_BUTTON_10
    },
    {
        PIN_EXTI_1, PIN_EXTI_2, PIN_EXTI_3, PIN_EXTI_4, PIN_EXTI_5,
        PIN_EXTI_6, PIN_EXTI_7, PIN_EXTI_8, PIN_EXTI_9, PIN_EXTI_10
    }
};
