#include "stm32_conf.h"
#include "can.h"

rcc_class rcc;
usart_class usart(&usart_cfg);
load_class load(&load_cfg);

void ReceiveMeas(void *pvParameters) 
{
    usart.Init();
    uint8_t buf8[8]; // buffer for receiving measurement data (2x float)
    uint32_t buf32[2]; // buffer for conversion data from 8bit to 32bit
    while(1){
        vTaskDelay(20);
        if(usart.Receive(buf8,8)){ 
            buf32[0] = ((buf8[0] << 24) | (buf8[1] << 16) | (buf8[2] << 8) | buf8[3]);
            buf32[1] = ((buf8[4] << 24) | (buf8[5] << 16) | (buf8[6] << 8) | buf8[7]);
            load.voltage = static_cast<float>(buf32[0]);
            load.current = static_cast<float>(buf32[1]);
        }
    }
}

void SendMeas(void *pvParameters) 
{
    while(1){
    }
}

void ReceiveCmd(void *pvParameters) 
{
    while(1){
    }
}

void LoadCmd(can_t *can)
{
    uint16_t load_mask;
    if(can->from != CTRL_COMP_ADDR || can->from != LOAD_ADDR) return;
    switch(can->cmd) {
        case LOAD_CMD_OFF:
            load.Off();
            // can_tx(can->to, can->from, can->cmd, 1, NULL, 1);
            break;
        case LOAD_CMD_ON:
            load.On();
            // can_tx(can->to, can->from, can->cmd, 1, NULL, 1);
            break;
        case LOAD_CMD_SET:
            load_mask = (can->data[0] << 8) | (can->data[1]);
            load.Set(load_mask);
            // can_tx(can->to, can->from, can->cmd, 1, NULL, 1);
            break;
        default:
            break;
    } 
}

int main(void) 
{
    HAL_Init();
    rcc.InitClock();
    can_init(LoadCmd);
    xTaskCreate(ReceiveMeas, "ReceiveMeas", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(SendMeas, "SendMeas", configMINIMAL_STACK_SIZE, NULL, CAN_TX_TASK_PRIO, NULL);
    xTaskCreate(ReceiveCmd, "ReceiveCmd", configMINIMAL_STACK_SIZE, NULL, CAN_RX_TASK_PRIO, NULL);
    vTaskStartScheduler();
    while(1){  
    }
}

extern "C" 
{
    void USART3_IRQHandler(void) 
    {
        usart.Handler();
    }
}
