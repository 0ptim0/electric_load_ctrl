#include "stm32f103xb.h"

#include "can.h"
#include "can_hw.h"

#include "FreeRTOS.h"


/*******************************************************************************
**                                                                            **
**                                 Interrupts                                 **
**                                                                            **
*******************************************************************************/

// #if CAN_TX
// void USB_HP_CAN1_TX_IRQHandler() {
    
//     // Отправляем уведомление задаче
//     can_tx_sem_give_isr();
    
//     // Очищаем флаги прерывания
//     CAN1->TSR |= CAN_TSR_RQCP0 | CAN_TSR_RQCP1 | CAN_TSR_RQCP2;
//     NVIC_ClearPendingIRQ(USB_HP_CAN1_TX_IRQn);
    
// }
// #endif

#if CAN_RX
void USB_LP_CAN1_RX0_IRQHandler() {
    
    can_hw_t packet;
    
    while (CAN1->RF0R & CAN_RF0R_FMP0) {
        
        // Формируем пакет
        packet.ext = (CAN1->sFIFOMailBox[0].RIR & CAN_RI0R_IDE) ? 1 : 0;
        packet.id  = CAN1->sFIFOMailBox[0].RIR >> (packet.ext ? 3 : 21);
        packet.len = CAN1->sFIFOMailBox[0].RDTR & 0x0F;
        CAN_UINT(packet.data,   32) = CAN1->sFIFOMailBox[0].RDLR;
        CAN_UINT(packet.data+4, 32) = CAN1->sFIFOMailBox[0].RDHR;
        
        // Отправляем пакет на обработку
        can_rx_irq(&packet);
        
        // Релиз буфера FIFO 0
        CAN1->RF0R |= CAN_RF0R_RFOM0;
        
    }
    
    // Очищаем флаг прерывания
    NVIC_ClearPendingIRQ(USB_LP_CAN1_RX0_IRQn);
    
}
#endif

/*******************************************************************************
**                                                                            **
**                                 Functions                                  **
**                                                                            **
*******************************************************************************/

void can_hw_init(void) {
    
    // Настраиваем регистры
    RCC->APB1ENR |= RCC_APB1ENR_CAN1EN; // Enable CAN clock
    CAN1->MCR = CAN_MCR_INRQ; // Enter initialization mode
    while (!(CAN1->MSR & CAN_MSR_INAK)); // And wait
    CAN1->MCR |= CAN_MCR_ABOM | CAN_MCR_TXFP; // Automatic bus-off management, transmit FIFO priority by the request order
    CAN1->IER = CAN_IER_FMPIE0 | CAN_IER_TMEIE; // FIFO0 buffer message pending and transmit mailbox empty interrupts enable
    CAN1->BTR = CAN_BAUDRATE; // Set baudrate
    
    // Тестовый режим или режим тишины
    #if CAN_TEST_MODE
    
        CAN1->BTR |= CAN_BTR_LBKM | CAN_BTR_SILM;
    
    #elif !CAN_RX
    
        CAN1->BTR |= CAN_BTR_SILM;
    
    #endif
    
    #if CAN_TX
        
        // TX interrupt
        NVIC_SetPriority(USB_HP_CAN1_TX_IRQn, CAN_TX_IRQ_PRIO);
        NVIC_EnableIRQ(USB_HP_CAN1_TX_IRQn);
        
    #endif
    
    #if CAN_RX
        
        // RX interrupt
        NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, CAN_RX_IRQ_PRIO);
        NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
        
    #endif
    
    // Стартуем CAN-контроллер
    CAN1->MCR &= ~CAN_MCR_INRQ;
    while (CAN1->MSR & CAN_MSR_INAK);
    
}

#if CAN_TX    
void can_hw_tx(can_hw_t * packet) {
    
    uint8_t n;
    
    // Если есть пустые мэйлбоксы, просто забираем семафор, иначе блокируем задачу до освобождения мэйлбокса
    can_tx_sem_take((CAN1->TSR & CAN_TSR_TME) == 0);
    
    // Получаем свободный мэйлбокс
    n = (CAN1->TSR & CAN_TSR_CODE) >> 24;
    
    // Заполняем данные
    CAN1->sTxMailBox[n].TDLR = CAN_UINT(packet->data,   32);
    CAN1->sTxMailBox[n].TDHR = CAN_UINT(packet->data+4, 32);
    CAN1->sTxMailBox[n].TDTR = packet->len;
    
    // Заполняем идентификатор и дергаем бит отправки
    CAN1->sTxMailBox[n].TIR = packet->ext
                            ? ((packet->id <<  3) | CAN_TI0R_TXRQ | CAN_TI0R_IDE)
                            : ((packet->id << 21) | CAN_TI0R_TXRQ);
    
}
#endif

#if CAN_RX
int32_t can_hw_filter(int32_t n, uint32_t id, uint32_t id_mask, uint8_t ext, uint8_t ext_mask) {
    
    static int32_t nn;
    
    if (n < 0) {
        n = nn++;
    }
    
    // Check filter aviability
    if (n > 27) {
        return -1;
    }
    
    // Set filter
    CAN1->FMR |= CAN_FMR_FINIT; // Set Initialisation mode for filter banks
    CAN1->sFilterRegister[n].FR1 = (id      << 3) | (ext      ? CAN_RI0R_IDE : 0);
    CAN1->sFilterRegister[n].FR2 = (id_mask << 3) | (ext_mask ? CAN_RI0R_IDE : 0);
    CAN1->FS1R |= 1<<n; // 32-bit mode
    CAN1->FA1R |= 1<<n; // Activate filter
    CAN1->FMR &= ~CAN_FMR_FINIT; // Reset Initialisation mode for filter banks
    
    return n;
    
}
#endif
