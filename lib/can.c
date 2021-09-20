#include <stdint.h>

#include "can.h"
#include "can_hw.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"

/*******************************************************************************
**                                                                            **
**                                 Variables                                  **
**                                                                            **
*******************************************************************************/

#if CAN_TX
    TaskHandle_t      can_tx_task_handle;
    QueueHandle_t     can_tx_queue[CAN_PRIORITIES];
    SemaphoreHandle_t can_tx_mutex[CAN_PRIORITIES];
    SemaphoreHandle_t can_tx_semphr;
#endif

#if (CAN_TX && CAN_TX_CONFIRM)
    can_confirm_t * can_confirm;
#endif

#if CAN_RX
    QueueHandle_t can_rx_queue;
#endif

/*******************************************************************************
**                                                                            **
**                                   Tasks                                    **
**                                                                            **
*******************************************************************************/

#if CAN_RX
void can_rx_task(void * param) {
    
    can_hw_t hw;
    can_t    buf[CAN_PRIORITIES] = {0};
    
    uint8_t  from;
    uint8_t  to;
    uint8_t  cmd;
    uint8_t  prio;
    uint8_t  p_num; // Номер пакета
    uint16_t b_num; // Номер первого байта
    uint16_t bytes; // Количество байт
    
    while (1) {
        
        // Получаем сообщение
        xQueueReceive(can_rx_queue, &hw, portMAX_DELAY);
        
        // Парсим номер первого байта данных
        p_num = (~(hw.id >> (24-CAN_PRIO_BITS))) & 0x1F;
        b_num = p_num << 3;
        if (b_num >= CAN_LENGTH) {
            continue;
        }
        
        // Парсим из регистров входные данные
        from  = hw.id >> (16 - CAN_PRIO_BITS);
        to    = hw.id >> ( 8 - CAN_PRIO_BITS);
        cmd   = ((hw.id >> 21) & CAN_PRIO_MASK) | (hw.id & CAN_CMD_MASK);
        prio  = can_cmd_prio(cmd);
        bytes = hw.len & 0x0F;
        #if CAN_9_BYTE_HACK
            if (bytes > 8) {
                bytes = 8;
            }
        #endif
        
        // Сохраняем заголовок пакета при приеме первого куска и проверяем при последующих
        // Выкидываем из id бит IDE (расширенного идентификатора)
        if (!p_num) {
            buf[prio].from = from;
            buf[prio].to   = to;
            buf[prio].cmd  = cmd;
            buf[prio].len  = 0;
        }
        else {
            if (from!=buf[prio].from || to!=buf[prio].to || cmd!=buf[prio].cmd) {
                continue;
            }
        }
        
        // Копируем данные
        buf[prio].len += bytes;
        if (bytes) {
            *(uint32_t *)(buf[prio].data + b_num)     = CAN_UINT(hw.data,   32);
            *(uint32_t *)(buf[prio].data + b_num + 4) = CAN_UINT(hw.data+4, 32);
        }
        
        // Вызываем функцию-обработчик, если пакет последний и все байты приняты
        if (((hw.len & 0x0F)!=8 || b_num==(CAN_LENGTH-8)) && buf[prio].len==(b_num+bytes)) {
            
            #if (CAN_TX && CAN_TX_CONFIRM)
                can_tx_confirm_update(buf[prio].from, buf[prio].cmd);
            #endif
            
            // Call processing callback
            ((void (*)(can_t *))param)(buf+prio);
            
        }
        
    }
    
}
#endif

#if CAN_TX
void can_tx_task(void * param) {
    
    uint16_t length;
    uint16_t i = 0;
    uint16_t len[CAN_PRIORITIES] = {0};
    uint32_t id[CAN_PRIORITIES];
    
    can_tx_t tx;
    can_hw_t hw;
    
    while (1) {
        
        // Отправка "завершающего" пакета?
        if (!CAN_9_BYTE_HACK && len[i]==0x8000) {
            
            // Обнуляем счетчик длины
            length = 0;
            len[i] = 0;
            
        }
        else {
            
            // Принимаем пакет из очереди
            xSemaphoreTake(can_tx_semphr, portMAX_DELAY);
            for (i=0; i<CAN_PRIORITIES; i++) {
                if (xQueueReceive(can_tx_queue[i], &tx, 0)) {
                    break;
                }
            }
            
            // Пришли данные или заголовок
            if (len[i]) {
                
                // Определяем длину пакета и декрементируем счетчик оставшихся данных
                if (len[i] > 8) {
                    length  = 8;
                    len[i] -= 8;
                }
                else if (CAN_9_BYTE_HACK && len[i]==8) {
                    length = 9;
                    len[i] = 0;
                }
                else {
                    length = len[i];
                    len[i] = 0;
                }
                
            }
            else {
                
                // Составляем ID
                id[i] = can_id(tx.from, tx.to, tx.cmd) | (0x1F000000 >> CAN_PRIO_BITS);
                
                // Есть данные в пакете?
                if (tx.len) {
                    
                    // Cохраняем длину пакета и переходим к следующему элементу в очереди без отправки
                    len[i] = tx.len;
                    if (!CAN_9_BYTE_HACK && !(tx.len&0x07) && tx.len<CAN_LENGTH) { // Хак не применяется, кратно 8 и не равно максимальной длине: надо будет "завершать" посылку пустым пакетом
                        len[i] |= 0x8000;
                    }
                    continue;
                    
                }
                else {
                    
                    // Будем отправлять пакет без данных
                    length = 0;
                    
                }
                
            }
            
        }
        
        // Формируем и отправляем пакет с данными
        hw.id = id[i];
        hw.len = length;
        hw.ext = 1;
        CAN_UINT(hw.data,   32) = tx.data[0];
        CAN_UINT(hw.data+4, 32) = tx.data[1];
        can_hw_tx(&hw);
        
        // Декрементируем id
        id[i] -= 0x01000000 >> CAN_PRIO_BITS;
        
    }
    
}
#endif

#if (CAN_TX && CAN_TX_CONFIRM)
void can_confirm_enable(uint8_t addr, uint8_t cmd) {
    
    can_confirm_t ** c = &can_confirm;
    
    // Walk by confirm objects
    while (*c) {
        
        // Check confirmation
        if ((*c)->task == xTaskGetCurrentTaskHandle()) {
            (*c)->en   = 1;
            (*c)->addr = addr;
            (*c)->cmd  = cmd;
            return;
        }
        
        // Go to next
        c = (can_confirm_t **)(&((*c)->next));
        
    }
    
    // Not found - allocate memory and fill data
    *c = pvPortMalloc(sizeof(can_confirm_t));
    (*c)->next = NULL;
    (*c)->task = xTaskGetCurrentTaskHandle();
    (*c)->en   = 1;
    (*c)->addr = addr;
    (*c)->cmd  = cmd;
    
}

void can_confirm_disable(void) {
    
    can_confirm_t * c = can_confirm;
    
    // Walk by confirm objects
    while (c) {
        
        // Check confirmation
        if (c->task == xTaskGetCurrentTaskHandle()) {
            c->en = 0;
            return;
        }
        
        // Go to next
        c = c->next;
        
    }
    
}
#endif

/*******************************************************************************
**                                                                            **
**                   Public functions for HW implementation                   **
**                                                                            **
*******************************************************************************/

#if CAN_TX

// Данной функцией сообщаем функции can_hw_tx() о возможности дальнейшей отправки пакетов
void can_tx_sem_give(void) {
    
    xTaskNotifyGive(can_tx_task_handle);
    
}

// Версия функции can_tx_sem_give() для прерываний
void can_tx_sem_give_isr(void) {
    
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    
    vTaskNotifyGiveFromISR(can_tx_task_handle, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    
}

// Данную функцию вызываем в can_hw_tx(). Если wait==1, семафор
// будет ожидаться бесконечно, иначе он просто очистится
void can_tx_sem_take(uint8_t wait) {
    
    ulTaskNotifyTake(pdTRUE, wait ? portMAX_DELAY : 0);
    
}

#endif

#if CAN_RX

// При приеме CAN-пакета передаем его в обработку с помощью этой функции
void can_rx(can_hw_t * packet) {
    
    xQueueSend(can_rx_queue, packet, portMAX_DELAY);
    
}

// Версия функции can_rx() для прерываний
void can_rx_irq(can_hw_t * packet) {
    
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    
    xQueueSendFromISR(can_rx_queue, packet, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    
}

#endif

/*******************************************************************************
**                                                                            **
**                              Public functions                              **
**                                                                            **
*******************************************************************************/

void can_init(void (*callback)(can_t *)) {
    
    #if !CAN_TX && !CAN_RX
        #error "Должен быть активен TX и/или RX"
    #endif
    
    #if CAN_TX
        
        // TX task
        xTaskCreate(can_tx_task, "cant", CAN_TX_TASK_STACK, NULL, CAN_TX_TASK_PRIO, &can_tx_task_handle);
        
        // TX queues and semaphores
        const uint32_t tx_len[CAN_PRIORITIES] = CAN_TX_QUEUE_LEN;
        uint32_t tx_len_sum = 0;
        
        for (uint8_t i=0; i<CAN_PRIORITIES; i++) {
            can_tx_mutex[i] = xSemaphoreCreateMutex();
            can_tx_queue[i] = xQueueCreate(tx_len[i], sizeof(can_tx_t));
            tx_len_sum += tx_len[i];
        }
        can_tx_semphr = xSemaphoreCreateCounting(tx_len_sum, 0);
        
    #endif
    
    #if CAN_RX
        
        // RX task and queue
        xTaskCreate(can_rx_task, "canr", CAN_RX_TASK_STACK, callback, CAN_RX_TASK_PRIO, NULL);
        can_rx_queue = xQueueCreate(CAN_RX_QUEUE_LEN, sizeof(can_hw_t));
        
    #endif
    
    // Инициализируем аппаратную часть
    can_hw_init();
    
}

#if CAN_TX
void can_tx(uint8_t from, uint8_t to, uint8_t cmd, uint16_t len, uint8_t * data, uint32_t wait) {
    
    uint16_t i;
    can_tx_t tx;
    uint8_t  prio = can_cmd_prio(cmd);
    
    // Контролируем длину
    if (len>CAN_LENGTH) {
        len = CAN_LENGTH;
    }
    
    // Забираем мьютекс
    xSemaphoreTake(can_tx_mutex[prio], portMAX_DELAY);
    
    // А есть ли место?
    if (wait || uxQueueSpacesAvailable(can_tx_queue[prio]) > (len / 8 + ((len % 8) ? 2 : 1))) {
        
        // Отправляем заголовок в задачу отправки
        tx.from = from;
        tx.to   = to;
        tx.cmd  = cmd;
        tx.len  = len;
        xQueueSend(can_tx_queue[prio], &tx, portMAX_DELAY);
        xSemaphoreGive(can_tx_semphr);
        
        // Отправляем данные в задачу отправки
        for (i=0; i<len; i+=8) {
            tx.data[0] = *((uint32_t *)(data+i));
            tx.data[1] = *((uint32_t *)(data+i+4));
            xQueueSend(can_tx_queue[prio], &tx, portMAX_DELAY);
            xSemaphoreGive(can_tx_semphr);
        }
        
    }
    
    // Отдаем мьютекс
    xSemaphoreGive(can_tx_mutex[prio]);
    
}
#endif

#if (CAN_TX && CAN_TX_CONFIRM)
uint32_t can_tx_confirm(uint8_t from, uint8_t to, uint8_t cmd, uint16_t len, uint8_t * data, uint32_t retries, uint32_t timeout) {
    
    can_confirm_enable(to, cmd);
    
    while (retries--) {
        xTaskNotifyStateClear(NULL);
        can_tx(from, to, cmd, len, data, 1);
        if (ulTaskNotifyTake(pdTRUE, timeout)) {
            return 1;
        }
    }
    
    return 0;
    
}

void can_tx_confirm_update(uint8_t addr, uint8_t cmd) {
    
    can_confirm_t * c = can_confirm;
    
    // Walk by confirm objects
    while (c) {
        
        // Check confirmation
        if (c->en && (!c->addr || c->addr == addr) && c->cmd == cmd) {
            c->en = 0;
            xTaskNotifyGive(c->task);
        }
        
        // Go to next
        c = c->next;
        
    }
    
}
#endif

#if CAN_RX
int32_t can_filter_cmd(int32_t n, uint8_t from, uint8_t from_mask, uint8_t to, uint8_t to_mask, uint8_t cmd, uint8_t cmd_mask) {
    
    // Перенаправляем на хардварную реализацию
    return can_hw_filter(n, can_id(from, to, cmd), can_id(from_mask, to_mask, cmd_mask), 1, 1);
    
}
#endif
