#include "stm32_conf.h"
#include "can.h"

rcc_class rcc;
usart_class usart(&usart_cfg);
load_class load(&load_cfg);
wake_class wake;
wake_packet_t tm;
uint8_t data[8];
gpio_class can_rx_pin(&can_rx_pin_cfg);
gpio_class can_tx_pin(&can_tx_pin_cfg);

static uint32_t a;
static uint32_t c;

// #define PIN_3     {GPIOB, {GPIO_PIN_3, GPIO_MODE_OUTPUT_OD, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}
// const gpio_cfg_t test3 = PIN_3;
// gpio_class pin_test3(&test3);

void ReceiveMeas(void *pvParameters) // TODO Timeout
{
    usart.Init();
    tm.data = data;
    tm.max_data_length = 8;
    while(1) {
            usart.Receive(wake.GetBufPtr(),wake.GetBufSize());
            a++;
            if(!wake.Unpacking(&tm)) {
                c++;
                load.tm.V_mV = ((tm.data[0]) | (tm.data[1] << 8) | (tm.data[2] << 16) | (tm.data[3] << 24));
                load.tm.I_mA = ((tm.data[4]) | (tm.data[5] << 8) | (tm.data[6] << 16) | (tm.data[7] << 24));
            }
    }
}

void SendMeas(void *pvParameters) 
{
    uint8_t test;
    test = 0xFF;
    can_rx_pin.Init();
    can_tx_pin.Init();
    while(1){
        vTaskDelay(1000);
    }
}

void LoadCmd(can_t *can)
{
    uint16_t load_mask;

    switch(can->cmd) {

        case LOAD_DISARM:
            load.Off();
            load.tm.state = DISARM;
            can_tx(can->to, can->from, can->cmd, 1, NULL, 1);
            break;

        case LOAD_MANUAL_CONTROL:
            load.Off();
            load.tm.state = MANUAL;
            can_tx(can->to, can->from, can->cmd, 1, NULL, 1);
            break;

        case LOAD_MIXED_CONTROL:
            load.Off();
            load.tm.state = MIXED;
            can_tx(can->to, can->from, can->cmd, 1, NULL, 1);
            break;

        case LOAD_REMOTE_CONTROL:
            load.Off();
            load.tm.state = REMOTE;
            can_tx(can->to, can->from, can->cmd, 1, NULL, 1);
            break;

        case LOAD_SET:
            if(load.tm.state != REMOTE && load.tm.state != MIXED) {
                can_tx(can->to, can->from, can->cmd, 1, NULL, 1);
                break;
            }
            load_mask = (can->data[1] << 8) | (can->data[0]);
            load.Set(load_mask);
            can_tx(can->to, can->from, can->cmd, 1, NULL, 1);
            break;

        case LOAD_TM:
            can_tx(can->to, can->from, can->cmd, 12, (uint8_t *)&load.tm, 1);
            break;

        default:
            can_tx(can->to, can->from, can->cmd, 1, NULL, 1);
            break;
    }
}

int main(void) 
{
    HAL_Init();
    __HAL_RCC_AFIO_CLK_ENABLE();
    __HAL_AFIO_REMAP_SWJ_NOJTAG();

    rcc.InitClock();

    can_init(LoadCmd);
    can_filter_cmd(0, 0x00, 0x00, LOAD_ADDR, 0xFF, 0x00, 0x00);

    load.tm.state = MANUAL;
    load.tm.error = LOAD_NO_ERR;

    xTaskCreate(ReceiveMeas, "ReceiveMeas", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(SendMeas, "SendMeas", configMINIMAL_STACK_SIZE, NULL, CAN_TX_TASK_PRIO, NULL);
    vTaskStartScheduler();
    while(1){  
    }
}

extern "C" 
{
    void USART2_IRQHandler(void) 
    {
        usart.Handler();
    }
}
