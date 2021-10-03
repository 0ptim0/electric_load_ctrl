#include "stm32_conf.h"
#include "can.h"

rcc_class rcc;
usart_class usart(&usart_cfg);
load_class load(&load_cfg);
wake_class wake;
wake_packet_t tm;
uint8_t data[8];

TimerHandle_t xOneShotTimer; 

void ReceiveMeas(void *pvParameters) 
{
    usart.Init();
    tm.data = data;
    while(1){
            usart.Receive(wake.GetBufPtr(),wake.GetBufSize());
            if(!wake.Unpacking(&tm)) {
                load.voltage = ((tm.data[0]) | (tm.data[1] << 8) | (tm.data[2] << 16) | (tm.data[3] << 24));
                load.current = ((tm.data[4]) | (tm.data[5] << 8) | (tm.data[6] << 16) | (tm.data[7] << 24));
                wake.ClearBuf();
            }
            while(0);
    }
}

void SendMeas(void *pvParameters) 
{
    while(1){
        vTaskDelay(1000);
    }
}

void ReceiveCmd(void *pvParameters) 
{
    while(1){
        vTaskDelay(1000);
    }
}

void LoadCmd(can_t *can)
{
    uint16_t load_mask;
    if(can->from != CTRL_COMP_ADDR || can->from != LOAD_ADDR) return;
    switch(can->cmd) {
        case LOAD_CMD_OFF:
            load.Off();
            can_tx(can->to, can->from, can->cmd, 1, NULL, 1);
            break;
        case LOAD_CMD_ON:
            load.On();
            can_tx(can->to, can->from, can->cmd, 1, NULL, 1);
            break;
        case LOAD_CMD_SET:
            load_mask = (can->data[0] << 8) | (can->data[1]);
            load.Set(load_mask);
            can_tx(can->to, can->from, can->cmd, 1, NULL, 1);
            break;
        default:
            break;
    }
    vTaskDelay(1000);
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
    void USART2_IRQHandler(void) 
    {
        usart.Handler();
    }
}
