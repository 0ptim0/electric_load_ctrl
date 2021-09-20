#ifndef _CAN_H
#define _CAN_H

#include "stm32_base.h"

// Settings

#define CAN_BAUDRATE      CAN_BAUDRATE_100KHZ

#define CAN_PRIO_BITS       2 /* ���������� ��� ����������. ����� �������� 2^CAN_PRIO_BITS ������� ��� ������. �������� 6 (��-�� ���������� �������������� �������������� � ����������� �������) */
#define CAN_LENGTH        256 /* ������������ ����� ������ (�������� 256, ������ 8) */

#define CAN_TEST_MODE       0
#define CAN_9_BYTE_HACK     1 /* ������������ ��� � ������������ ���������� ����� ������ 9 ���� ��� �������� ���������� ������ (����� ��� ����� ���������� ������ 8 ���� ����� ���� ���������� ������ �����) */

#define CAN_TX              1
#define CAN_TX_CONFIRM      1
#define CAN_TX_TASK_PRIO    6
#define CAN_TX_TASK_STACK  64 /* �� ������������ �� �������, ����� ���������� ���� */
#define CAN_TX_QUEUE_LEN  { 128, 128, 128, 128 } /* ���������� ����-������� (������ 8 ����) �� �������� ��� ������� ���������� */
#define CAN_TX_IRQ_PRIO   ( (configMAX_SYSCALL_INTERRUPT_PRIORITY >> (8 - __NVIC_PRIO_BITS)) + 1 )

#define CAN_RX              1
#define CAN_RX_TASK_PRIO    7
#define CAN_RX_TASK_STACK ( CAN_LENGTH * CAN_PRIORITIES / 4 + 256 ) /* ������ ������ ������, ������� �� ��������� ������� ��������� ������� */
#define CAN_RX_QUEUE_LEN  256 /* ���������� ���������� (������ 16 ����) ������� �� ����� */
#define CAN_RX_IRQ_PRIO   ( (configMAX_SYSCALL_INTERRUPT_PRIORITY >> (8 - __NVIC_PRIO_BITS)) + 2 )

// Defines

#define CAN_PRIORITIES ( 1 << CAN_PRIO_BITS )
#define CAN_CMD_MASK   ( (uint8_t)(0xFF >> CAN_PRIO_BITS) )
#define CAN_PRIO_MASK  ( (uint8_t)(~(0xFF >> CAN_PRIO_BITS)) )

#define can_id(from, to, cmd) ( \
    ( (uint32_t)(        from         ) << (16 - CAN_PRIO_BITS) ) | \
    ( (uint32_t)(         to          ) << ( 8 - CAN_PRIO_BITS) ) | \
    ( (uint32_t)((cmd) & CAN_PRIO_MASK) << 21 ) | \
      (uint32_t)((cmd) & CAN_CMD_MASK ) \
)

#define can_cmd_prio(cmd) ( cmd >> (8 - CAN_PRIO_BITS) )

// Typedefs

typedef __packed struct {
    uint32_t id;
    uint8_t  len:4;
    uint8_t  ext:1;
    uint8_t  data[8];
} can_hw_t;

#pragma anon_unions
typedef union {
    struct {
        uint8_t  from;
        uint8_t  to;
        uint8_t  cmd;
        uint16_t len;
    };
    uint32_t data[2];
} can_tx_t;

typedef struct {
    uint8_t  from;
    uint8_t  to;
    uint8_t  cmd;
    uint16_t len;
    uint8_t  data[CAN_LENGTH];
} can_t;

#if (CAN_TX && CAN_TX_CONFIRM)
    typedef struct {
        void * next;
        TaskHandle_t task;
        uint8_t en;
        uint8_t addr;
        uint8_t cmd;
    } can_confirm_t;
#endif


// Macroses

#define CAN_CONCAT3(a, b, c) a ## b ## c
#define  CAN_INT(ptr, bits) (*((CAN_CONCAT3( int, bits, _t) *)(ptr)))
#define CAN_UINT(ptr, bits) (*((CAN_CONCAT3(uint, bits, _t) *)(ptr)))

// Public functions for HW implementation

void can_rx(can_hw_t * packet);
void can_rx_irq(can_hw_t * packet);
void can_tx_sem_give(void);
void can_tx_sem_give_isr(void);
void can_tx_sem_take(uint8_t wait);

// Public functions

void can_init(void (*callback)(can_t *));

#if CAN_TX
    void can_tx(uint8_t from, uint8_t to, uint8_t cmd, uint16_t len, uint8_t * data, uint32_t wait);
#endif

#if (CAN_TX && CAN_TX_CONFIRM)
    uint32_t can_tx_confirm(uint8_t from, uint8_t to, uint8_t cmd, uint16_t len, uint8_t * data, uint32_t retries, uint32_t timeout);
    void can_tx_confirm_update(uint8_t addr, uint8_t cmd);
#endif

#if CAN_RX
    #define can_filter(n, from, from_mask, to, to_mask) { can_filter_cmd((n), (from), (from_mask), (to), (to_mask), 0x00, 0x00); }
    int32_t can_filter_cmd(int32_t n, uint8_t from, uint8_t from_mask, uint8_t to, uint8_t to_mask, uint8_t cmd, uint8_t cmd_mask);
#endif

#endif
